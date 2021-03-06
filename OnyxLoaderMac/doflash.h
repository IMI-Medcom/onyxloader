/*
	Simple example to open a maximum of 4 devices - write some data then read it back.
	Shows one method of using list devices also.
	Assumes the devices have a loopback connector on them and they also have a serial number

	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o simple main.c -L. -lftd2xx -Wl,-rpath /usr/local/lib
*/

#include "stm32ld/stm32ld.h"
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include <time.h>

#include <errno.h>
#include <limits.h>

#include "ftd2xx.h"
#include "jsmn.h"

#define BL_VERSION_MAJOR  2
#define BL_VERSION_MINOR  1
#define BL_MKVER( major, minor )    ( ( major ) * 256 + ( minor ) ) 
#define BL_MINVERSION               BL_MKVER( BL_VERSION_MAJOR, BL_VERSION_MINOR )

#define CHIP_ID           0x0414

#define BUF_SIZE 0x10

FT_HANDLE	ftHandle[MAX_DEVICES];
char 	        cBufLD[MAX_DEVICES][64];
int             foundDevices = 0;
extern int stm32_ser_id;
int ser_dbg = 0;

static  FILE *fp_page1;
static  FILE *fp_page4;
static  u32 fpsize;

int fw_abort();

#define CBUS_RESET_MASK   0x1
#define CBUS_BOOT_MASK    0x4

int getandcheckCBUS( FT_HANDLE ftHandle0 ) {
  FT_PROGRAM_DATA Data;
  int need_write = 0;
  FT_STATUS	ftStatus;

  if( ser_dbg )
    printf("ftHandle0 = %p\n", ftHandle0);

  Data.Signature1 = 0x00000000;
  Data.Signature2 = 0xffffffff;
  Data.Manufacturer = (char *)malloc(256);		// "FTDI"
  Data.ManufacturerId = (char *)malloc(256);	// "FT"
  Data.Description = (char *)malloc(256);			// "USB HS Serial Converter"
  Data.SerialNumber = (char *)malloc(256);		// "FT000001" if fixed, or NULL
  ftStatus = FT_EE_Read(ftHandle0, &Data);
  if(ftStatus != FT_OK) {
    printf("FT_EE_Read failed\n");
    FT_Close(ftHandle0);
    return 1;
  }

  if( ser_dbg ) {
    printf("Cbus0 = 0x%X\n", Data.Cbus0);				// Cbus Mux control
    printf("Cbus1 = 0x%X\n", Data.Cbus1);				// Cbus Mux control
    printf("Cbus2 = 0x%X\n", Data.Cbus2);				// Cbus Mux control
    printf("Cbus3 = 0x%X\n", Data.Cbus3);				// Cbus Mux control
    printf("Cbus4 = 0x%X\n", Data.Cbus4);				// Cbus Mux control
  }

  // check that cbus0 and 2 are write
  if( Data.Cbus0 != 0x0A ) {
    printf( "Cbus0 is %d, should be %d, updating!\n", Data.Cbus0, 0xA );
    Data.Cbus0 = 0x0A;
    need_write = 1;
  }
  
  if( Data.Cbus2 != 0x0A ) {
    printf( "Cbus2 is %d, should be %d, updating!\n", Data.Cbus2, 0xA );
    Data.Cbus2 = 0x0A;
    need_write = 1;
  }

  // check that CBUS3 is power enable
  if( Data.Cbus3 != 0x01 ) {
    printf( "Cbus3 is %d, should be %d, updating!\n", Data.Cbus3, 0x1);
    Data.Cbus2 = 0x0B;
    need_write = 1;
  }

  // not necessary, but for the hell of it, cbus 1 is read
  if( Data.Cbus1 != 0x0A ) {
    printf( "Cbus1 is %d, should be %d, updating!\n", Data.Cbus1, 0xA );
    Data.Cbus1 = 0x0A;
    need_write = 1;
  }

  if( need_write ) {
    printf( "Updating EEPROM to correct setting for safecast.\n" );
    ftStatus = FT_EE_Program(ftHandle0, &Data);
    if(ftStatus != FT_OK) {
      printf("FT_EE_Program failed (%d)\n", ftStatus);
      FT_Close(ftHandle0);
      return 1;
    }
    printf( "------> Now that the EEPROM is updated, unplug and replug the device.\n" );
  } else {
    printf( "EEPROM values are up to date, not modifying them\n" );
  }
  free(Data.Manufacturer);
  free(Data.ManufacturerId);
  free(Data.Description);
  free(Data.SerialNumber);
  return 0;

}


int openSerialPorts8N1(int baud) {
    char * 	pcBufRead = NULL;
    char * 	pcBufLD[MAX_DEVICES + 1];
    //DWORD	dwRxSize = 0; (unused)
    //DWORD 	dwBytesWritten, dwBytesRead; (unused)
    FT_STATUS	ftStatus;
    int	iNumDevs = 0;
    int	i;  //, j; (unused)
    int	iDevicesOpen = 0;
    // unsigned char ucMode = 0x00; (unused)
    
    printf( "warning: opening up to %d ports and assuming all are Safecast devices.\n", MAX_DEVICES );
    printf( "todo: make this more selective and safer.\n" );
    
    for(i = 0; i < MAX_DEVICES; i++) {
        pcBufLD[i] = cBufLD[i];
    }
    pcBufLD[MAX_DEVICES] = NULL;
    
    ftStatus = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);
    
    if(ftStatus != FT_OK) {
        fprintf(stderr,"Error: FT_ListDevices(%d)\n", ftStatus);
        return -1;
    }
  
    if (iNumDevs == 0) {
        fprintf(stderr, "There aren't any FT devices\n");
        return -1;
    }
  
    for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) {
        printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
    }
    
    for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ) ; i++) {
        /* Setup */
        if((ftStatus = FT_OpenEx(cBufLD[i], FT_OPEN_BY_SERIAL_NUMBER, &ftHandle[i])) != FT_OK){
            /*
             This can fail if the ftdi_sio driver is loaded
             use lsmod to check this and rmmod ftdi_sio to remove
             also rmmod usbserial
             */
            fprintf(stderr,"Error FT_OpenEx(%d), device %d\n", ftStatus, i);
            return -1;
        }
        
        printf("Opened device %s\n", cBufLD[i]);
        
        if(getandcheckCBUS(ftHandle[i]) ) {
            printf( "getandcheckCBUS failed, exiting.\n" );
            return -1;
        }
        
        iDevicesOpen++;
        if((ftStatus = FT_SetBaudRate(ftHandle[i], baud)) != FT_OK) {
            fprintf(stderr,"Error FT_SetBaudRate(%d), cBufLD[i] = %s\n", ftStatus, cBufLD[i]);
            break;
        }
        
        if((ftStatus = FT_SetDataCharacteristics(ftHandle[i], FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE)) != FT_OK) {
            fprintf(stderr,"Error FT_SetDataCharacteristics(%d) = %s\n", ftStatus, cBufLD[i]);
            break;
        }
        
        FT_SetTimeouts(ftHandle[i], 5000, 5000);	// 500 ms read/write timeout
        
    }
    
    iDevicesOpen = i;
    foundDevices = i; // record this in a global
    
    if(pcBufRead)
        free(pcBufRead);
    
    return 0; // we always use the 0th device for now
}

int openSerialPorts(int baud) {
  char * 	pcBufRead = NULL;
  char * 	pcBufLD[MAX_DEVICES + 1];
  //DWORD	dwRxSize = 0; (unused) 
  //DWORD 	dwBytesWritten, dwBytesRead; (unused) 
  FT_STATUS	ftStatus;
  int	iNumDevs = 0;
  int	i; //, j; (unused)
  int	iDevicesOpen = 0;
  // unsigned char ucMode = 0x00; (unused)

  printf( "warning: opening up to %d ports and assuming all are Safecast devices.\n", MAX_DEVICES );
  printf( "todo: make this more selective and safer.\n" );

  for(i = 0; i < MAX_DEVICES; i++) {
    pcBufLD[i] = cBufLD[i];
  }
  pcBufLD[MAX_DEVICES] = NULL;
  
  ftStatus = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);
  
  if(ftStatus != FT_OK) {
    fprintf(stderr,"Error: FT_ListDevices(%d)\n", ftStatus);
    return -1;
  }
  
  for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) {
    printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
  }
  
  for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ) ; i++) {
    /* Setup */
    if((ftStatus = FT_OpenEx(cBufLD[i], FT_OPEN_BY_SERIAL_NUMBER, &ftHandle[i])) != FT_OK){
      /* 
	 This can fail if the ftdi_sio driver is loaded
	 use lsmod to check this and rmmod ftdi_sio to remove
	 also rmmod usbserial
      */
      fprintf(stderr,"Error FT_OpenEx(%d), device %d\n", ftStatus, i);
      return -1;
    }
    
    printf("Opened device %s\n", cBufLD[i]);
    
    if(getandcheckCBUS(ftHandle[i]) ) {
      printf( "getandcheckCBUS failed, exiting.\n" );
      return -1;
    }
    
    iDevicesOpen++;
    if((ftStatus = FT_SetBaudRate(ftHandle[i], baud)) != FT_OK) {
      fprintf(stderr,"Error FT_SetBaudRate(%d), cBufLD[i] = %s\n", ftStatus, cBufLD[i]);
      break;
    }

    if((ftStatus = FT_SetDataCharacteristics(ftHandle[i], FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_EVEN)) != FT_OK) {
      fprintf(stderr,"Error FT_SetDataCharacteristics(%d) = %s\n", ftStatus, cBufLD[i]);
      break;
    }
    
    FT_SetTimeouts(ftHandle[i], 5000, 5000);	// 500 ms read/write timeout

  }
  
  iDevicesOpen = i;
  foundDevices = i; // record this in a global

  if(pcBufRead)
    free(pcBufRead);

  return 0; // we always use the 0th device for now
}

// mode = 1 goes to bootloader, mode = 0 goes to normal
void safecast_resetboard(int mode) {
  unsigned char mask = 0;
  unsigned char ucMode;
  FT_STATUS	ftStatus;

  if( mode == 1 ) {
    mask = CBUS_BOOT_MASK;
  } else {
    mask = 0;
  }
  printf( "Resetting MCU and forcing " );
  if( mode == 1 ) {
    printf( "System Mode." );
  } else {
    printf( "normal mode." );
  }
  fflush(stdout);

  ucMode = 0xF0 | mask; // set to system memory mode
  ftStatus = FT_SetBitMode(ftHandle[stm32_ser_id], ucMode, 0x20); // CBUS bitbang mode
  if(ftStatus != FT_OK) {
    printf("Failed to set Bit Mode\n");
  } else {
    if( ser_dbg )
      printf("Set bitbang mode to %02x\n", ucMode );
  }
  usleep(500000);
  printf( "." );
  fflush(stdout);
  
  ucMode = 0xF0 | mask | CBUS_RESET_MASK; // release reset
  ftStatus = FT_SetBitMode(ftHandle[stm32_ser_id], ucMode, 0x20); // CBUS bitbang mode
  if(ftStatus != FT_OK) {
    printf("Failed to set Bit Mode\n");
  } else {
    if( ser_dbg )
      printf("Set bitbang mode to %02x\n", ucMode );
  }
  usleep(500000);
  if( mode ) 
    printf( ".should now be in system mode.\n" );
  else
    printf( ".should now be running as normal.\n" );
  
}

int closeSerialPorts() {
  int i;
  FT_STATUS	ftStatus;

  /* Cleanup */
  for(i = 0; i < foundDevices; i++) {
    if((ftStatus = FT_SetDataCharacteristics(ftHandle[i], FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE)) != FT_OK) {
      printf("Error FT_SetDataCharacteristics(%d) = %s\n", ftStatus, cBufLD[i]);
      break;
    }
    FT_Close(ftHandle[i]);
    printf("Closed device %s\n", cBufLD[i]);
  }
  return 0;
}

void printhelp(char *name, char *note) {
  printf( "Usage: %s \n", name );
}

int fw_abort() {
  closeSerialPorts();
  return 0;
}

// Get data function
static u32 writeh_read_data_page1( u8 *dst, u32 len )
{
  size_t readbytes = 0;

  if( !feof( fp_page1 ) )
    readbytes = fread( dst, 1, len, fp_page1 );
  return ( u32 )readbytes;
}

// Get data function
static u32 writeh_read_data_page4( u8 *dst, u32 len )
{
  size_t readbytes = 0;

  if( !feof( fp_page4 ) )
    readbytes = fread( dst, 1, len, fp_page4 );
  return ( u32 )readbytes;
}

// Progress function
int expected_next=10;
static void writeh_progress( u32 wrote)
{
  unsigned long pwrite = ( wrote * 100 ) / fpsize;

  if( pwrite >= expected_next )
  {
    printf( "%d%% ", expected_next );
    expected_next += 10;
  }
}


char *read_to_prompt(int id) {

  char *data = (char *) malloc(10000);
  int   data_size  = 10000;
  int   wrote_size = 0;
    
    
  int b=0;
  int lb=0;
  for(;;) {
    lb = b;
    b = ser_read_byte(id);
    if(b == -1) return data;
      
    data[wrote_size] = b;
    data[wrote_size+1] = 0;
    wrote_size++;
    //printf("read: %d %c\n",b,b);
    if(wrote_size == (data_size-5)) {
        data = (char *) realloc(data,data_size+10000);
        data_size += 10000;
    }
      
    if((lb == 10) && (b == '>')) return data;
  }
    
}

char *do_get_log_csv() {
    
    // open serial ports
    int id = openSerialPorts8N1(115200);
  
    if (id == -1)
      return NULL;
    
    ser_write(id,(const u8 *) "\r\n\r\n",4);
    sleep(1);
    ser_set_timeout_ms( id, SER_NO_TIMEOUT );
    while( ser_read_byte(id) != -1 );
    ser_set_timeout_ms( id, STM32_COMM_TIMEOUT );
    
    
    printf("device id: %d\n",id);
    
    // Send Pause log
    ser_write(id,(const u8 *) "LOGPAUSE\n",9);
    free(read_to_prompt(id));
    
    // Send LOGXFER
    ser_write(id,(const u8 *) "LOGXFER\n",8);
    char *logdata = read_to_prompt(id);
    
    // Send Resume log
    ser_write(id,(const u8 *) "LOGRESUME\n",10);
    free(read_to_prompt(id));
        
    // close serial ports
    closeSerialPorts();
    
    char *alldata = (char *) malloc(strlen(logdata)+100);
    
    strcpy(alldata,logdata);
    size_t loglen = strlen(alldata);
    alldata[loglen]=10;
    alldata[loglen+1]=13;
    alldata[loglen+2]=0;
    
    //parse json
    
    jsmn_parser parser;
    jsmn_init(&parser);
    
    jsmntok_t *tokens = (jsmntok_t *) malloc(sizeof(jsmntok_t)*1000000);
    
    int r = jsmn_parse(&parser, alldata, tokens, 1000000);
    
    char *outdata = (char *) malloc(10000000);
    char *outdatal = outdata;
    if(r != JSMN_SUCCESS) {
      strcpy(outdata,"READFAILED");
    } else {
    
       bool processing=false;
       for(size_t n=0;n<1000000;n++) {
          if(tokens[n].type == JSMN_ARRAY) {
            processing = true;
            printf("detect array: %ld\n",n);
          }
          
          if(processing) {
          
            if(tokens[n].type == JSMN_STRING) {
            
              char *s = (char *) malloc(tokens[n].end-tokens[n].start);
              strncpy(s,alldata+tokens[n].start,tokens[n].end-tokens[n].start);
              
              if(strcmp(s,"time") == 0) {
                strncpy(outdatal,alldata+tokens[n+1].start,tokens[n+1].end-tokens[n+1].start);
                outdatal+=(tokens[n+1].end-tokens[n+1].start);
                outdatal[0]=',';
                outdatal[1]=0;
                outdatal++;
              }

              if(strcmp(s,"cpm") == 0) {
                strncpy(outdatal,alldata+tokens[n+1].start,tokens[n+1].end-tokens[n+1].start);
                outdatal+=(tokens[n+1].end-tokens[n+1].start);
                outdatal[0]='\n';
                outdatal[1]=0;
                outdatal++;
              }

            }
          
          }
       }
       outdatal[0]=0;
    
    }
    
    free(logdata);
    free(tokens);
    free(alldata);

    return outdata;
}

int do_set_time() {

  time_t t = time(NULL);

  int id = openSerialPorts8N1(115200);
  
  if (id == -1)
    return 0;
  
  ser_write(id,(const u8 *) "SETRTC\n",7);
  sleep(1);

  char stime[100];
  sprintf(stime,"%lu\n",t);
  
  size_t length = strlen(stime);
  int result = 1;
  
  if (ser_write(id,(const u8 *) stime, length) != length)
    result = 0;

  closeSerialPorts();
  return result;
}

char *do_get_log() {
    
    // open serial ports
    int id = openSerialPorts8N1(115200);
  
    if (id == -1)
      return NULL;
    
    ser_write(id,(const u8 *) "\r\n\r\n",4);
    sleep(1);
    ser_set_timeout_ms( id, SER_NO_TIMEOUT );
    while( ser_read_byte(id) != -1 );
    ser_set_timeout_ms( id, STM32_COMM_TIMEOUT );
    
    
    printf("device id: %d\n",id);
    
    // Send Pause log
    ser_write(id,(const u8 *) "LOGPAUSE\n",9);
    free(read_to_prompt(id));
    
    // Send LOGXFER
    ser_write(id,(const u8 *) "LOGXFER\n",8);
    char *logdata = read_to_prompt(id);
    
    // Send LOGSIGN
    ser_set_timeout_ms( id, SER_INF_TIMEOUT );
    ser_write(id,(const u8 *) "LOGSIG\n",8);
    char *logsig = read_to_prompt(id);
    ser_set_timeout_ms( id, STM32_COMM_TIMEOUT );
    
    // Send Resume log
    ser_write(id,(const u8 *) "LOGRESUME\n",10);
    free(read_to_prompt(id));
        
    // close serial ports
    closeSerialPorts();
    
    char *alldata = (char *) malloc(strlen(logdata)+strlen(logsig)+100);
    
    strcpy(alldata,logdata);
    size_t loglen = strlen(alldata);
    alldata[loglen]=10;
    alldata[loglen+1]=13;
    strcpy(alldata+loglen+2,logsig);
    
    free(logdata);
    free(logsig);
    return alldata;
}

int do_flash_main(int argc, char **argv) {

  int aflag = 0;
  char *argval = NULL;
  char infile_name[256];
  //int index; (unused)
  int c;
  long baud = 115200;
  u8 minor, major;
  u16 version;
  int badness = 0;
  int readflag = 0;
  long readoffset = 0;

  opterr = 0;
  infile_name[0] = '\0';
  ser_dbg = 0;

  //reset optind as we may call multiple times.
  //opterr=0;
  //optopt=0;
  optind=1;

  while ((c = getopt(argc, argv, "b:f:dr:")) != -1 ) {
    switch (c) {
      
    case 'a': 
      aflag = 1;
      break;
      
    case 'b':
      argval = optarg;
      baud = strtol(argval, NULL, 0);
      if( baud < 1200 || baud > 115200 ) {
	printf( "Baud should be between 1200 and 115200; got: %ld\n", baud );
	return 0;
      }
      break;
      
    case 'f':
      strncpy(infile_name, optarg, 256);
      break;
      
    case 'd':
      ser_dbg = 1;
      break;
      
    case 'r':
      argval = optarg;
      readoffset = strtol(argval, NULL, 0);
      readflag = 1;
      break;
      
    case '?':
      printhelp(argv[0], NULL);
      break;

    default:
      printhelp(argv[0], NULL);
      fw_abort();
    }
  }

  if( !readflag ) {
    fp_page1 = fopen( infile_name, "rb" );
    fp_page4 = fopen( infile_name, "rb" );

    if((fp_page1 == NULL) || (fp_page4 == NULL)) {
      fprintf( stderr, "Unable to open %s\n", infile_name );
      return 10;
    }  else    {
      fseek( fp_page4, 0, SEEK_END );
      fpsize = ftell( fp_page4 );
      fseek( fp_page4, 0, SEEK_SET );
    }
    fseek( fp_page4, 6144, SEEK_SET);
  }

  // Connect to bootloader
  printf( "Connect to bootloader.\n" );
  if( stm32_init( NULL, baud ) != STM32_OK )
  {
    fprintf( stderr, "Unable to connect to bootloader\n" );
  }
  printf( "\n" );
  
  // Get version
  printf( "Get version.\n" );
  if( stm32_get_version( &major, &minor ) != STM32_OK )
  {
    fprintf( stderr, "Unable to get bootloader version\n" );
    //    exit( 1 );
    badness = 1;
  }
  else
  {
    printf( "Found bootloader version: %d.%d\n", major, minor );
    if( BL_MKVER( major, minor ) < BL_MINVERSION )
    {
      fprintf( stderr, "Unsupported bootloader version" );
      return 11;
    }
  }
  
  // Get chip ID
  printf( "Get chip ID.\n" );
  if( stm32_get_chip_id( &version ) != STM32_OK )
  {
    fprintf( stderr, "Unable to get chip ID\n" );
    badness = 1;
    //    exit( 1 );
  }
  else
  {
    printf( "Chip ID: %04X\n", version );
    if( version != CHIP_ID )
    {
      fprintf( stderr, "Unsupported chip ID" );
      return 13;
    }
  }
  if( badness )
    return 14;
  
  if( !readflag ) {
    // Write unprotect
    printf( "Write unprotect.\n" );
    if( stm32_write_unprotect() != STM32_OK )
      {
	fprintf( stderr, "Unable to execute write unprotect\n" );
	return 15;
      }
    else
      printf( "Cleared write protection.\n" );

    // Read unprotect
    printf( "Read unprotect.\n" );
    if( stm32_read_unprotect() != STM32_OK )
      {
	fprintf( stderr, "Unable to execute read unprotect\n" );
	return 16;
      }
    else
      printf( "Cleared read protection.\n" );

    // Erase flash
    printf( "Erase flash.\n" );
    int res1 = stm32_erase_flash_page(0,1);     // first page
    int res2 = stm32_erase_flash_page(3,0xFD);  // all the rest
    if(res1 != STM32_OK) {
	    fprintf( stderr, "Unable to erase chip - pre prvkey\n" );
	    return 16;
    }
    if(res2 != STM32_OK) {
	    fprintf( stderr, "Unable to erase chip - post pk\n" );
	    return 17;
    }

    printf( "Erased FLASH memory.\n" );


    // Program flash
    setbuf( stdout, NULL );
    expected_next=10;
    printf( "Programming flash ... ");
    if( stm32_write_flash_page(0x08000000,1,writeh_read_data_page1, writeh_progress ) != STM32_OK ) {
      fprintf( stderr, "Unable to program - initial page.\n" );
	    return 18;
    } else {
      printf( "\nProgram pre-pk Done.\n" );
    }

    if( stm32_write_flash_page(0x08001800,0xFC,writeh_read_data_page4, writeh_progress ) != STM32_OK ) {
      fprintf( stderr, "Unable to program - post pk flash.\n" );
	    return 19;
    } else {
      printf( "\nProgram post-pk Done.\n" );
    }


  } else {
    printf( "Readback flash memory at offset %lx\n", readoffset );
    if(stm32_read_flash( readoffset, 10240 ) != STM32_OK ) {
      fprintf( stderr, "Unable to read FLASH memory.\n" );
      return 20;
    } else {
      printf( "\nDone.\n" );
    }
  }

  // reset back into normal mode
  printf( "\n" );
  safecast_resetboard(0);

  if( !readflag ) {
    fclose( fp_page1 );
    fclose( fp_page4 );
  }

  closeSerialPorts();

  return 0;
}

const char *map_flash_error_to_string(int error) {
  switch (error) {
    case 0:
      return "No Error";
    case 10:
      return "Unable to open file for programming";
    case 11:
      return "Unsupported bootloader version";
    case 13:
      return "Unsupported chip ID";
    case 14:
      return "Unable to get bootloader version or Chip ID (communication failure)";
    case 15:
      return "Unable to execute write unprotect";
    case 16:
      return "Unable to execute read unprotect or erase chip pre prvkey";
    case 17:
      return "Unable to erase chip - post pk";
    case 18:
      return "Unable to program - initial page";
    case 19:
      return "Unable to program - post pk flash";
    case 20:
      return "Unable to read FLASH memory";
    default:
      return NULL;
  }
}

