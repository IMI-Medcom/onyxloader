#pragma once

#include "doflash.h"
#include "gen_util.h"

#include <Vcclr.h>
#include <wininet.h>

#include <msclr/marshal.h>
using namespace msclr::interop;

#pragma comment(lib, "wininet.lib")

#define IN_DEBUG_MODE 0

namespace TryUSB
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Text;
	using namespace System::Globalization;
    using namespace System::IO;
    using namespace System::Text;
//	const UInt32 FT_LIST_NUMBER_ONLY = 0x80000000;
//	const UInt32 FT_LIST_BY_INDEX	= 0x40000000;
//	const UInt32 FT_LIST_ALL			= 0x20000000;
//	const UInt32 FT_OPEN_BY_SERIAL_NUMBER = 1;
//	const UInt32 FT_OPEN_BY_DESCRIPTION    = 2;

//	const UInt32 FT_EVENT_RXCHAR		= 1;
//	const UInt32 FT_EVENT_MODEM_STATUS  = 2;

#define FT_PREFIX [DllImport("FTD2XX.dll")]

enum {
    FT_OK,
    FT_INVALID_HANDLE,
    FT_DEVICE_NOT_FOUND,
    FT_DEVICE_NOT_OPENED,
    FT_IO_ERROR,
    FT_INSUFFICIENT_RESOURCES,
    FT_INVALID_PARAMETER,
    FT_INVALID_BAUD_RATE,
    FT_DEVICE_NOT_OPENED_FOR_ERASE,
    FT_DEVICE_NOT_OPENED_FOR_WRITE,
    FT_FAILED_TO_WRITE_DEVICE,
    FT_EEPROM_READ_FAILED,
    FT_EEPROM_WRITE_FAILED,
    FT_EEPROM_ERASE_FAILED,
    FT_EEPROM_NOT_PRESENT,
    FT_EEPROM_NOT_PROGRAMMED,
    FT_INVALID_ARGS,
    FT_NOT_SUPPORTED,
    FT_OTHER_ERROR
};

	typedef void * FT_HANDLE;
	typedef unsigned long DWORD;
	typedef unsigned long FT_STATUS;
	typedef void * LPVOID;
	typedef void * PVOID;
	typedef DWORD * LPDWORD;
	typedef DWORD ULONG;
	typedef unsigned short USHORT;
	typedef unsigned char UCHAR;
	typedef unsigned short WORD;
	typedef WORD * LPWORD;
	typedef unsigned char UCHAR;
	typedef UCHAR * PUCHAR;
	typedef char CHAR;
	typedef CHAR * PCHAR;
	typedef ULONG FT_DEVICE;
	typedef void *HANDLE;
	typedef int BOOL;
	#define FALSE   0
	#define TRUE    1
	// as c++. net is a managed application and our ftd2xx.dll is unmanaged code you must declare the functions here explicitly
	// to allow you to call them within the application. An include file and the .lib file simply wont work with c++.net. Its
	// a similar problem in c#.
	// see http://www.codeguru.com/Cpp/COM-Tech/complus/managed/article.php/c3947/ for more on this.
	// I have only included these 4 functions in this to show you how to do this. For other functions that you require similar
	// declerations will need to go here. 
 
	using namespace System::Runtime::InteropServices;
	FT_PREFIX FT_STATUS FT_Open(int deviceNumber, FT_HANDLE * pHandle);
	FT_PREFIX FT_STATUS FT_OpenEx(PVOID pArg1, DWORD Flags, FT_HANDLE *pHandle);
	FT_PREFIX FT_STATUS FT_ListDevices(PVOID pArg1, PVOID pArg2, DWORD Flags);
	FT_PREFIX FT_STATUS FT_ListDevices(UInt32 pvArg1, void * pvArg2, UInt32 dwFlags);	// FT_ListDevcies by serial number or description by index only
	FT_PREFIX FT_STATUS FT_Close(FT_HANDLE ftHandle);
	FT_PREFIX FT_STATUS FT_Read(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesReturned);
	FT_PREFIX FT_STATUS FT_Write(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesWritten);
	FT_PREFIX FT_STATUS FT_SetBaudRate(FT_HANDLE ftHandle,	ULONG BaudRate);
	FT_PREFIX FT_STATUS FT_SetDivisor(FT_HANDLE ftHandle,USHORT Divisor);
	FT_PREFIX FT_STATUS FT_SetDataCharacteristics(FT_HANDLE ftHandle, UCHAR WordLength, UCHAR StopBits, UCHAR Parity);
	FT_PREFIX FT_STATUS FT_SetFlowControl(FT_HANDLE ftHandle, USHORT FlowControl, UCHAR XonChar, UCHAR XoffChar	);
	FT_PREFIX FT_STATUS FT_ResetDevice(FT_HANDLE ftHandle);
	FT_PREFIX FT_STATUS FT_SetDtr(FT_HANDLE ftHandle);
	FT_PREFIX FT_STATUS FT_ClrDtr(FT_HANDLE ftHandle);
	FT_PREFIX FT_STATUS FT_SetRts(FT_HANDLE ftHandle);
	FT_PREFIX FT_STATUS FT_ClrRts(FT_HANDLE ftHandle);
	FT_PREFIX FT_STATUS FT_GetModemStatus(FT_HANDLE ftHandle,	ULONG *pModemStatus);
	FT_PREFIX FT_STATUS FT_SetChars(FT_HANDLE ftHandle, UCHAR EventChar, UCHAR EventCharEnabled, UCHAR ErrorChar, UCHAR ErrorCharEnabled);
	FT_PREFIX FT_STATUS FT_Purge(FT_HANDLE ftHandle, ULONG Mask);
	FT_PREFIX FT_STATUS FT_SetTimeouts(FT_HANDLE ftHandle,	ULONG ReadTimeout,	ULONG WriteTimeout);
	FT_PREFIX FT_STATUS FT_GetQueueStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes);
	FT_PREFIX FT_STATUS FT_SetEventNotification(FT_HANDLE ftHandle, DWORD Mask,	PVOID Param	);
	FT_PREFIX FT_STATUS FT_GetStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes, DWORD *dwTxBytes, DWORD *dwEventDWord);
	FT_PREFIX FT_STATUS FT_SetBreakOn(FT_HANDLE ftHandle);
	FT_PREFIX FT_STATUS FT_SetBreakOff(FT_HANDLE ftHandle);	
	FT_PREFIX FT_STATUS FT_SetWaitMask(FT_HANDLE ftHandle, DWORD Mask);
	FT_PREFIX FT_STATUS FT_WaitOnMask(FT_HANDLE ftHandle, DWORD *Mask);
	FT_PREFIX FT_STATUS FT_GetEventStatus(FT_HANDLE ftHandle, DWORD *dwEventDWord);
	FT_PREFIX FT_STATUS FT_ReadEE(FT_HANDLE ftHandle, DWORD dwWordOffset, LPWORD lpwValu);
	FT_PREFIX FT_STATUS FT_WriteEE(FT_HANDLE ftHandle,	DWORD dwWordOffset, WORD wValue);
	FT_PREFIX FT_STATUS FT_EraseEE(FT_HANDLE ftHandle);
// Missed out the programming stuff +++ 

	FT_PREFIX FT_STATUS  FT_EE_UASize(FT_HANDLE ftHandle, LPDWORD lpdwSize);
	FT_PREFIX FT_STATUS  FT_EE_UAWrite(FT_HANDLE ftHandle, PUCHAR pucData, DWORD dwDataLen	);
	FT_PREFIX FT_STATUS  FT_EE_UARead(FT_HANDLE ftHandle, PUCHAR pucData, DWORD dwDataLen, LPDWORD lpdwBytesRead);
	FT_PREFIX FT_STATUS  FT_SetLatencyTimer(FT_HANDLE ftHandle, UCHAR ucLatency);
	FT_PREFIX FT_STATUS  FT_GetLatencyTimer(FT_HANDLE ftHandle, PUCHAR pucLatency);
	FT_PREFIX FT_STATUS  FT_SetBitMode(FT_HANDLE ftHandle, UCHAR ucMask, UCHAR ucEnable);
	FT_PREFIX FT_STATUS  FT_GetBitMode(FT_HANDLE ftHandle, PUCHAR pucMode);
	FT_PREFIX FT_STATUS  FT_SetUSBParameters(FT_HANDLE ftHandle, ULONG ulInTransferSize, ULONG ulOutTransferSize);
	FT_PREFIX FT_STATUS  FT_GetDeviceInfo(FT_HANDLE ftHandle, FT_DEVICE *lpftDevice, LPDWORD lpdwID, PCHAR SerialNumber, PCHAR Description,	LPVOID Dummy);
	FT_PREFIX FT_STATUS  FT_StopInTask(FT_HANDLE ftHandle);
	FT_PREFIX FT_STATUS  FT_RestartInTask(FT_HANDLE ftHandle);
	FT_PREFIX FT_STATUS  FT_SetResetPipeRetryCount(FT_HANDLE ftHandle, DWORD dwCount);
	FT_PREFIX FT_STATUS  FT_ResetPort(FT_HANDLE ftHandle);

	// need these kernel functions for the Event Handling stuff
	[DllImport("Kernel32.dll")] DWORD WaitForSingleObject(HANDLE hHandle,  DWORD dwMilliseconds);
	[DllImport("Kernel32.dll")] HANDLE CreateEvent(void * pNULL, BOOL bManualReset, BOOL bInitialState, char * pcNULL);
	[DllImport("Kernel32.dll")] BOOL SetEvent(HANDLE hEvent);
// missed out the W32 stuff as well

	FT_HANDLE handle;
	HANDLE hEvent;
	/// <summary> 
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
    public ref class Form1 : public System::Windows::Forms::Form
	{	
	public:
		Form1(void)
		{
			InitializeComponent();
            InitializeBackgoundWorker();
		}

	private:
		void ReadingProc()
        {
			ArrayList ^ palReadData;
			palReadData = gcnew ArrayList();
			TimeSpan waitTime = TimeSpan(0, 0, 1);	// 1 second timeout
			static char c = 0;
			char* pstr6 = &c;
			FT_STATUS status;

			while(bContinue) {
				DWORD dwRead, dwRXBytes;
				Byte b;
				NumberFormatInfo^   provider = gcnew NumberFormatInfo( );

				WaitForSingleObject(hEvent, -1);
				if(handle) {
					status = FT_GetQueueStatus(handle, &dwRead);
					if(status != FT_OK) {
						MessageBox::Show("GError");
						continue;
					}
					while(dwRead && bContinue) {
						status = FT_Read(handle, &b, 1, &dwRXBytes);
						if(status != FT_OK) {
							MessageBox::Show("RError");
							continue;
						}
						else {
					//		String* s;
					//		s = Convert::ToString(b);
					//		listBox1->Items->Add(s);
						}
						status = FT_GetQueueStatus(handle, &dwRead);
					}
				}
				Thread::Sleep(0);
			}
			if(handle) {
				FT_Close(handle);
			}
        }

        ~Form1(){
            StopThread();		// Close our thread
            
            //RS - using CLI "Dispose" apparently maps to destructor

            bool disposing = true;
            if (disposing && components)
            {
                //components->Dispose();
            }
            //__super::Dispose(disposing);
        }
    protected:
        /*void Dispose(Boolean disposing)
        {
            StopThread();		// Close our thread

            if (disposing && components)
            {
                components->Dispose();
            }
            __super::Dispose(disposing);
        }*/

    private: System::Windows::Forms::Button ^  btnClose;
    private: System::Windows::Forms::Button ^  btnProgRelease;

    private: System::Threading::Thread ^ thrdReader;


    private: bool bContinue;
    private: UInt32 dwOpenFlags;
    private: ManualResetEvent ^ ev;
    private: System::Windows::Forms::Button ^  btnProgBeta;
    private: System::Windows::Forms::Button ^  btnProgExp;


    private: System::Windows::Forms::Button ^  btnSave;
    private: System::Windows::Forms::Button ^  btnSetTime;


    private: System::Windows::Forms::Label ^  label1;
    private: System::Windows::Forms::Label ^  label2;
    private: System::Windows::Forms::Button^  btnProgLocal;
    private: System::Windows::Forms::Label^  StatusLabel;
    private: System::Windows::Forms::ProgressBar^  progressBar1;
    private: System::Windows::Forms::Button^  btnTest;

    System::ComponentModel::BackgroundWorker^ backgroundWorkerTest;
    System::ComponentModel::BackgroundWorker^ backgroundWorkerRunFlash;
    System::ComponentModel::BackgroundWorker^ backgroundWorkerRunFlashLocal;

    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^ components;

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
            this->btnClose = (gcnew System::Windows::Forms::Button());
            this->btnProgRelease = (gcnew System::Windows::Forms::Button());
            this->btnProgBeta = (gcnew System::Windows::Forms::Button());
            this->btnProgExp = (gcnew System::Windows::Forms::Button());
            this->btnSave = (gcnew System::Windows::Forms::Button());
            this->btnSetTime = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->btnProgLocal = (gcnew System::Windows::Forms::Button());
            this->StatusLabel = (gcnew System::Windows::Forms::Label());
            this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
            this->btnTest = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // btnClose
            // 
            this->btnClose->Location = System::Drawing::Point(511, 51);
            this->btnClose->Name = L"btnClose";
            this->btnClose->Size = System::Drawing::Size(115, 10);
            this->btnClose->TabIndex = 1;
            this->btnClose->Text = L"Upload Log";
            this->btnClose->Click += gcnew System::EventHandler(this, &Form1::ButtonCancel_Click);
            // 
            // btnProgRelease
            // 
            this->btnProgRelease->Location = System::Drawing::Point(137, 25);
            this->btnProgRelease->Name = L"btnProgRelease";
            this->btnProgRelease->Size = System::Drawing::Size(138, 23);
            this->btnProgRelease->TabIndex = 2;
            this->btnProgRelease->Text = L"Latest Release";
            this->btnProgRelease->Click += gcnew System::EventHandler(this, &Form1::btnProgRelease_Click);
            // 
            // btnProgBeta
            // 
            this->btnProgBeta->Location = System::Drawing::Point(137, 54);
            this->btnProgBeta->Name = L"btnProgBeta";
            this->btnProgBeta->Size = System::Drawing::Size(138, 23);
            this->btnProgBeta->TabIndex = 3;
            this->btnProgBeta->Text = L"Beta Release";
            this->btnProgBeta->Click += gcnew System::EventHandler(this, &Form1::btnProgBeta_Click);
            // 
            // btnProgExp
            // 
            this->btnProgExp->Location = System::Drawing::Point(137, 83);
            this->btnProgExp->Name = L"btnProgExp";
            this->btnProgExp->Size = System::Drawing::Size(138, 23);
            this->btnProgExp->TabIndex = 4;
            this->btnProgExp->Text = L"Experimental Release";
            this->btnProgExp->Click += gcnew System::EventHandler(this, &Form1::btnProgExp_Click);
            // 
            // btnSave
            // 
            this->btnSave->Location = System::Drawing::Point(12, 25);
            this->btnSave->Name = L"btnSave";
            this->btnSave->Size = System::Drawing::Size(119, 23);
            this->btnSave->TabIndex = 5;
            this->btnSave->Text = L"Save Data";
            this->btnSave->Click += gcnew System::EventHandler(this, &Form1::btnSave_Click);
            // 
            // btnSetTime
            // 
            this->btnSetTime->Location = System::Drawing::Point(12, 54);
            this->btnSetTime->Name = L"btnSetTime";
            this->btnSetTime->Size = System::Drawing::Size(119, 23);
            this->btnSetTime->TabIndex = 6;
            this->btnSetTime->Text = L"Set Time";
            this->btnSetTime->Click += gcnew System::EventHandler(this, &Form1::btnSetTime_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(137, 9);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(87, 13);
            this->label1->TabIndex = 7;
            this->label1->Text = L"Update Firmware";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->label2->Location = System::Drawing::Point(0, 0);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(52, 12);
            this->label2->TabIndex = 8;
            this->label2->Text = L"Version 0.2";
            // 
            // btnProgLocal
            // 
            this->btnProgLocal->Location = System::Drawing::Point(137, 112);
            this->btnProgLocal->Name = L"btnProgLocal";
            this->btnProgLocal->Size = System::Drawing::Size(138, 23);
            this->btnProgLocal->TabIndex = 9;
            this->btnProgLocal->Text = L"Local Firmware";
            this->btnProgLocal->Click += gcnew System::EventHandler(this, &Form1::btnProgLocal_Click);
            // 
            // StatusLabel
            // 
            this->StatusLabel->AutoSize = true;
            this->StatusLabel->Location = System::Drawing::Point(12, 166);
            this->StatusLabel->MaximumSize = System::Drawing::Size(270, 0);
            this->StatusLabel->MinimumSize = System::Drawing::Size(270, 0);
            this->StatusLabel->Name = L"StatusLabel";
            this->StatusLabel->Size = System::Drawing::Size(270, 13);
            this->StatusLabel->TabIndex = 10;
            this->StatusLabel->Text = L"Idle, Device Not Connected";
            this->StatusLabel->TextAlign = System::Drawing::ContentAlignment::TopCenter;
            this->StatusLabel->Click += gcnew System::EventHandler(this, &Form1::StatusLabel_Click);
            // 
            // progressBar1
            // 
            this->progressBar1->Location = System::Drawing::Point(0, 145);
            this->progressBar1->Name = L"progressBar1";
            this->progressBar1->Size = System::Drawing::Size(287, 11);
            this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Marquee;
            this->progressBar1->TabIndex = 11;
            this->progressBar1->MarqueeAnimationSpeed = 0;
            // 
            // btnTest
            // 
            this->btnTest->Location = System::Drawing::Point(12, 112);
            this->btnTest->Name = L"btnTest";
            this->btnTest->Size = System::Drawing::Size(119, 23);
            this->btnTest->TabIndex = 12;
            this->btnTest->Text = L"Test";
            this->btnTest->Click += gcnew System::EventHandler(this, &Form1::btnTest_Click);
            // 
            // Form1
            // 
            this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
            this->ClientSize = System::Drawing::Size(287, 202);
            this->Controls->Add(this->btnTest);
            this->Controls->Add(this->progressBar1);
            this->Controls->Add(this->StatusLabel);
            this->Controls->Add(this->btnProgLocal);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->btnSetTime);
            this->Controls->Add(this->btnSave);
            this->Controls->Add(this->btnProgExp);
            this->Controls->Add(this->btnProgBeta);
            this->Controls->Add(this->btnProgRelease);
            this->Controls->Add(this->btnClose);
            this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
            this->Location = System::Drawing::Point(56, 48);
            this->MaximizeBox = false;
            this->MaximumSize = System::Drawing::Size(303, 240);
            this->MinimumSize = System::Drawing::Size(303, 240);
            this->Name = L"Form1";
            this->Text = L"OnyxLoader";
            this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void InitializeBackgoundWorker()
        {
            this->backgroundWorkerTest = gcnew System::ComponentModel::BackgroundWorker;
            backgroundWorkerTest->DoWork += gcnew DoWorkEventHandler(this, &Form1::backgroundWorkerTest_DoWork);
            backgroundWorkerTest->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &Form1::backgroundWorkerTest_RunWorkerCompleted);

            this->backgroundWorkerRunFlash = gcnew System::ComponentModel::BackgroundWorker;
            backgroundWorkerRunFlash->DoWork += gcnew DoWorkEventHandler(this, &Form1::backgroundWorkerRunFlash_DoWork);
            backgroundWorkerRunFlash->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &Form1::backgroundWorkerRunFlash_RunWorkerCompleted);

            this->backgroundWorkerRunFlashLocal = gcnew System::ComponentModel::BackgroundWorker;
            backgroundWorkerRunFlashLocal->DoWork += gcnew DoWorkEventHandler(this, &Form1::backgroundWorkerRunFlashLocal_DoWork);
            backgroundWorkerRunFlashLocal->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &Form1::backgroundWorkerRunFlashLocal_RunWorkerCompleted);

        }

	private: System::Void btnSave_Click(System::Object ^  sender, System::EventArgs ^  e)
			{
				
            SaveFileDialog^ sfd = gcnew SaveFileDialog();
            sfd->Filter = "CSV Files|*.csv|All Files|*.*";
			sfd->AddExtension = true;

			System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;

            if( sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK )
            {
                return;
            }

			FileStream ^ fs = gcnew FileStream(sfd->FileName,FileMode::Create);

				char *data = do_get_log_csv();

				for(int n=0;;n++) {
				  fs->WriteByte(data[n]);
				  if(data[n] == 0) break;
				}
				fs->Close();
				MessageBox::Show( "Save Complete" );
			}

	private: System::Void btnSetTime_Click(System::Object ^  sender, System::EventArgs ^  e)
			{
				do_set_time();
				MessageBox::Show( "Time Set" );
			}

	private: System::Void ButtonCancel_Click(System::Object ^  sender, System::EventArgs ^  e)
			{
				printf("Sending log\n");

				char *data = do_get_log();
				char *boundary = "0xKhTmLbOuNdArY";

				char *buffer = (char*) malloc (sizeof(char)*strlen(data) + 2048);
				char *hdrs   = (char*) malloc(2048);

			    //print header
			    sprintf(hdrs,"Content-Type: multipart/form-data; boundary=%s\r\n",boundary);
				sprintf(buffer,"--%s\r\n",boundary);
			    sprintf(buffer+strlen(buffer),"Content-Disposition: form-data; name=\"uploadedfile\"; filename=\"sc2\"\r\n");
				sprintf(buffer+strlen(buffer),"Content-Type: application/octet-stream\r\n\r\n");

			    sprintf(buffer+strlen(buffer),"%s\r\n",data);
			    sprintf(buffer+strlen(buffer),"--%s--\r\n",boundary);


				HINTERNET hSession = InternetOpen("WinSock",INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

			    HINTERNET hConnect = InternetConnect(hSession, "41j.com",INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);

			    HINTERNET hRequest = HttpOpenRequest(hConnect, (const char*)"POST","sc/sc.php", NULL, NULL, (const char**)"*/*\0", 0, 1);

			    BOOL sent= HttpSendRequest(hRequest, hdrs, strlen(hdrs), buffer, strlen(buffer));
			 
			    //close any valid internet-handles
			    InternetCloseHandle(hSession);
			    InternetCloseHandle(hConnect);
			    InternetCloseHandle(hRequest);

			}
	private: System::Void OpenPort() {
	  FT_STATUS ftStatus = FT_OK;
	  ftStatus = FT_Open(0, &handle);
	}

 	private: System::Void ClosePort() {
      if(handle) {
        FT_Close(handle);
		handle = NULL;
	  }
	}


    private: System::Void runFlash(char *szUrl) {
        // Download flash image from http://41j.com/
        UpdateUserMessage("Running, Firmware Is Downloading, Do Not Disconnect", false);
        backgroundWorkerRunFlash->RunWorkerAsync( char_star_to_system_string(szUrl) );
    }
    
    private: System::Void backgroundWorkerRunFlash_DoWork(Object^ sender, DoWorkEventArgs^ e) {
        
        BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
        System::String ^ sysUrl = e->Argument->ToString();

        marshal_context ^ context = gcnew marshal_context();
        const char* szUrl = context->marshal_as<const char*>(sysUrl);


        printf("url %s\n", szUrl);

        OutputDebugString(_T(szUrl));

        char szFileName[MAX_PATH];

        if (IN_DEBUG_MODE) {
            Sleep(4000);
        }
        else {
            HINTERNET hOpen = InternetOpen("WinSock",INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
            HANDLE hFile     = INVALID_HANDLE_VALUE;
            HANDLE hTempFile = INVALID_HANDLE_VALUE; 

            BOOL fSuccess  = FALSE;
            DWORD dwRetVal = 0;
            UINT uRetVal   = 0;

            DWORD dwBytesRead    = 0;
            DWORD dwBytesWritten = 0; 

            //char szFileName[MAX_PATH];  
            TCHAR lpTempPathBuffer[MAX_PATH];
            char  chBuffer[1024]; 


            // create temp file, the windows way
            //  Gets the temp path env string (no guarantee it's a valid path).
            dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
                               lpTempPathBuffer); // buffer for path 

            //  Generates a temporary file name. 
            uRetVal = GetTempFileName(lpTempPathBuffer, // directory for tmp files
                                  TEXT("DEMO"),     // temp file name prefix 
                                  0,                // create unique name 
                                  szFileName);      // buffer for name

            DWORD dwSize;
            CHAR   szHead[] = "Accept: */*\r\n\r\n";
            VOID * szTemp[25];
            HINTERNET  hConnect;
            FILE * pFile;

            if ( !(hConnect = InternetOpenUrl ( hOpen, szUrl, szHead,
             lstrlen (szHead), INTERNET_FLAG_DONT_CACHE, 0)))
            {
                //  cerr << "Error !" << endl;
                return ;
            }

            if  ( !(pFile = fopen (szFileName, "wb" ) ) )
            {
                //   cerr << "Error !" << endl;
                return ;
            }
            do
            {
                // Keep coping in 25 bytes chunks, while file has any data left.
                // Note: bigger buffer will greatly improve performance.
                if (!InternetReadFile (hConnect, szTemp, 50,  &dwSize) )
                {
                    fclose (pFile);
                    //    cerr << "Error !" << endl;
                    MessageBox::Show("Failed to download firmware image" );
                    return ;
                }
                if (!dwSize) {
                    break;  // Condition of dwSize=0 indicate EOF. Stop.
                }
                else {
                    fwrite(szTemp, sizeof (char), dwSize, pFile);
                }
            }   // do
            while (TRUE);
        
            fflush (pFile);
            fclose (pFile);
        }

        printf("filename %s\n", szFileName);

        e->Result = char_star_to_system_string(szFileName);

    } // end of runFlash()

    private: System::Void backgroundWorkerRunFlash_RunWorkerCompleted(Object^ sender, RunWorkerCompletedEventArgs^ e) {
        UpdateUserMessage("Running, Firmware Has Downloaded, Do Not Disconnect", true);
        UpdateUserMessage("Running, Firmware Is Being Flashed, Do Not Disconnect", false);
        backgroundWorkerRunFlashLocal->RunWorkerAsync(e->Result->ToString());
    }

    private: System::Void runFlashLocal(char* szFileName) {
        UpdateUserMessage("Running, Firmware Is Being Flashed, Do Not Disconnect", false);
        backgroundWorkerRunFlashLocal->RunWorkerAsync(char_star_to_system_string(szFileName));
    }

    private: System::Void backgroundWorkerRunFlashLocal_DoWork(Object^ sender, DoWorkEventArgs^ e) {
        // Flash    

        BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
        System::String ^ sysFileName = e->Argument->ToString();

        marshal_context ^ context = gcnew marshal_context();
        const char* cszFileName = context->marshal_as<const char*>(sysFileName);

        char szFileName[MAX_PATH];
        strcpy(szFileName, cszFileName);

        int argc=3;
        char *argv[3];
        
        char *argv0 = szFileName;
        char *argv1 = "-f";
        char *argv2 = (char *) szFileName;
        
        argv[0] = argv0;
        argv[1] = argv1;
        argv[2] = argv2;

        int result = 0;
        if (IN_DEBUG_MODE) {
            Sleep(4000);
        }
        else {
            result = do_flash_main(argc,argv);
        }
        if (result == 0) {
            MessageBox::Show("Flash Completed Successfully");
        }
        else {
            MessageBox::Show("Flash Programming failed");
        }
    }

    private: System::Void backgroundWorkerRunFlashLocal_RunWorkerCompleted(Object^ sender, RunWorkerCompletedEventArgs^ e) {
        UpdateUserMessage("Idle, Device Is Connected", true);
    }

    private: System::Void btnProgRelease_Click(System::Object ^  sender, System::EventArgs ^  e) {
        char *szUrl = "http://41j.com/safecast_latest.bin"; // URL
        runFlash(szUrl);
    }

    private: System::Void btnProgBeta_Click(System::Object ^  sender, System::EventArgs ^  e) {
        MessageBox::Show("Programming Beta Firmware: This firmware is for user testing only, and is not supported by Medcom International." );
        char *szUrl = "http://41j.com/safecast_beta.bin"; // URL
        runFlash(szUrl);
    }

    private: System::Void btnProgExp_Click(System::Object ^  sender, System::EventArgs ^  e) {
        MessageBox::Show("Programming Beta Firmware: This firmware is for user testing only, and is not supported by Medcom International." );
        char *szUrl = "http://41j.com/safecast_exp.bin"; // URL
        runFlash(szUrl);
    }

    private: System::Void btnProgLocal_Click(System::Object ^ sender, System::EventArgs ^  e) {
        MessageBox::Show("Programming Local Firmware: This firmware is for user testing only, and is not supported by Medcom International.");
        
        char szFileName[MAX_PATH];

        OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
        openFileDialog1->Filter = "Firmware Files|*.bin";
        openFileDialog1->Title = "Select a Onyx Firmware File";

        // Show the Dialog.
        // If the user clicked OK in the dialog and
        // a .bin file was selected, open it.
        if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            marshal_context ^ context = gcnew marshal_context();
            const char* FileName = context->marshal_as<const char*>(openFileDialog1->FileName);
            strcpy(szFileName, FileName);
            MessageBox::Show("Begin flashing this firmware to device:  " + openFileDialog1->FileName);
            
            runFlashLocal(szFileName);
        }

    }

    private: System::Void btnTest_Click(System::Object ^ sender, System::EventArgs ^  e) {
        UpdateUserMessage("start running test", false);
        backgroundWorkerTest->RunWorkerAsync();
    }

    private: System::Void backgroundWorkerTest_DoWork(Object^ sender, DoWorkEventArgs^ e) {
        // Get the BackgroundWorker that raised this event.
        BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);

        // Assign the result of the computation 
        // to the Result property of the DoWorkEventArgs 
        // object. This is will be available to the  
        // RunWorkerCompleted eventhandler.
        //e->Result = ComputeFibonacci(safe_cast<Int32>(e->Argument), worker, e);
        Sleep(4000);
    }

    private: System::Void backgroundWorkerTest_RunWorkerCompleted(Object^ /*sender*/, RunWorkerCompletedEventArgs^ e ) {
        UpdateUserMessage("end running test", true);
    }

    private: System::Void UpdateUserMessage(System::String ^ message, bool end) {       
        this->StatusLabel->Text = message;
        if(!end) {
            this->progressBar1->Style = ProgressBarStyle::Marquee;
            this->progressBar1->MarqueeAnimationSpeed = 30;
        }
        else {
            this->progressBar1->Style = ProgressBarStyle::Blocks;
        }
        
        //this->Invalidate();
        this->Update();
    }

    private: System::Void FillComboBox(UInt32 dwDescFlags) {
    
    }

    private: System::Void StopThread() {
        bContinue = false;
        if(hEvent)		// let the thread come out of waiting for infinite time
            SetEvent(hEvent);
        if(thrdReader && thrdReader->IsAlive) {	// stop it
            TimeSpan waitTime = TimeSpan(0, 0, 1);	// 1 second timeout
            if(thrdReader->Join(waitTime) != true) {
                thrdReader->Abort();	// didnt stop the thread - take more drastic action
            }
        }
    }

    private: System::Void StartThread() {
        // Create a reader thread here
        thrdReader = gcnew Thread(gcnew ThreadStart(this, &TryUSB::Form1::ReadingProc));
        bContinue = true;
        thrdReader->Start();
        while (!thrdReader->IsAlive);	// wait for the thread to start
        Thread::Sleep(1000);
    }

    private: System::Void Form1_Load(System::Object ^  sender, System::EventArgs ^  e) {	
        handle = NULL;
        hEvent = CreateEvent(NULL, FALSE, FALSE, "");
    }

    private: System::Void comboBox1_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e) {
        ClosePort(); 
        OpenPort();
    }

    private: System::Void radioNumber_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e) {
    }

    private: System::Void radioDescription_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e) {
    }

    private: System::Void radioSerial_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e) {
    }

    private: System::Void StatusLabel_Click(System::Object^  sender, System::EventArgs^  e) {
    }

};
}


