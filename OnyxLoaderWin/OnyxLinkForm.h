#pragma once

#include "doflash.h"
#include "gen_util.h"
#include "plot_data.h"
#include "CLI_FTD2XX.h"

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

    using namespace ZedGraph;

    // need these kernel functions for the Event Handling stuff
    [System::Runtime::InteropServices::DllImport("Kernel32.dll")] DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
    [System::Runtime::InteropServices::DllImport("Kernel32.dll")] HANDLE CreateEvent(void * pNULL, BOOL bManualReset, BOOL bInitialState, char * pcNULL);
    [System::Runtime::InteropServices::DllImport("Kernel32.dll")] BOOL SetEvent(HANDLE hEvent);
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
            UpdateUserMessage(get_connection_status_string() );
            StopProgressBar();
        }

    private:
        void ReadingProc() {
            ArrayList ^ palReadData;
            palReadData = gcnew ArrayList();
            TimeSpan waitTime = TimeSpan(0, 0, 1);  // 1 second timeout
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
                            //String* s;
                            //s = Convert::ToString(b);
                            //listBox1->Items->Add(s);
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
            StopThread();   // Close our thread
            
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
    private: System::Windows::Forms::Label^  OnyxLink_version_label;

    private: System::Windows::Forms::Label ^  label3;
    private: System::Windows::Forms::Button^  btnProgLocal;
    private: System::Windows::Forms::Label^  StatusLabel;
    private: System::Windows::Forms::ProgressBar^  progressBar1;
    private: System::Windows::Forms::Button^  btnTest;

    System::ComponentModel::BackgroundWorker^ backgroundWorkerTest;
    System::ComponentModel::BackgroundWorker^ backgroundWorkerRunFlash;
    System::ComponentModel::BackgroundWorker^ backgroundWorkerRunFlashLocal;
    System::ComponentModel::BackgroundWorker^ backgroundWorkerRunSaveData;
    System::ComponentModel::BackgroundWorker^ backgroundWorkerRunPlotData;

    private: ZedGraph::ZedGraphControl^  zedGraphControl1;
    private: System::Windows::Forms::Button^  btnPlot;
    private: System::ComponentModel::IContainer^  components;
    private: System::Windows::Forms::Button^  btnCloseGraph;

    //for restoring old width and height when closing graph
    private: int m_form_width;
    private: int m_form_height;
    private: System::Boolean m_adv_features_enabled;
    private: System::String^ m_cur_version_string;


    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>


        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
            this->btnClose = (gcnew System::Windows::Forms::Button());
            this->btnProgRelease = (gcnew System::Windows::Forms::Button());
            this->btnProgBeta = (gcnew System::Windows::Forms::Button());
            this->btnProgExp = (gcnew System::Windows::Forms::Button());
            this->btnSave = (gcnew System::Windows::Forms::Button());
            this->btnSetTime = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->OnyxLink_version_label = (gcnew System::Windows::Forms::Label());
            this->btnProgLocal = (gcnew System::Windows::Forms::Button());
            this->StatusLabel = (gcnew System::Windows::Forms::Label());
            this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
            this->btnTest = (gcnew System::Windows::Forms::Button());
            this->zedGraphControl1 = (gcnew ZedGraph::ZedGraphControl());
            this->btnPlot = (gcnew System::Windows::Forms::Button());
            this->btnCloseGraph = (gcnew System::Windows::Forms::Button());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();
            // 
            // btnClose
            // 
            this->btnClose->Location = System::Drawing::Point(1119, 12);
            this->btnClose->Name = L"btnClose";
            this->btnClose->Size = System::Drawing::Size(115, 10);
            this->btnClose->TabIndex = 1;
            this->btnClose->Text = L"Upload Log";
            this->btnClose->Click += gcnew System::EventHandler(this, &Form1::ButtonCancel_Click);
            // 
            // btnProgRelease
            // 
            this->btnProgRelease->BackColor = System::Drawing::Color::White;
            this->btnProgRelease->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnProgRelease->Location = System::Drawing::Point(189, 25);
            this->btnProgRelease->Name = L"btnProgRelease";
            this->btnProgRelease->Size = System::Drawing::Size(138, 23);
            this->btnProgRelease->TabIndex = 2;
            this->btnProgRelease->Text = L"Latest Release";
            this->btnProgRelease->UseVisualStyleBackColor = false;
            this->btnProgRelease->Click += gcnew System::EventHandler(this, &Form1::btnProgRelease_Click);
            // 
            // btnProgBeta
            // 
            this->btnProgBeta->BackColor = System::Drawing::Color::White;
            this->btnProgBeta->Enabled = false;
            this->btnProgBeta->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnProgBeta->Location = System::Drawing::Point(189, 54);
            this->btnProgBeta->Name = L"btnProgBeta";
            this->btnProgBeta->Size = System::Drawing::Size(138, 23);
            this->btnProgBeta->TabIndex = 3;
            this->btnProgBeta->Text = L"Beta Release";
            this->btnProgBeta->UseVisualStyleBackColor = false;
            this->btnProgBeta->Click += gcnew System::EventHandler(this, &Form1::btnProgBeta_Click);
            // 
            // btnProgExp
            // 
            this->btnProgExp->BackColor = System::Drawing::Color::White;
            this->btnProgExp->Enabled = false;
            this->btnProgExp->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnProgExp->Location = System::Drawing::Point(189, 83);
            this->btnProgExp->Name = L"btnProgExp";
            this->btnProgExp->Size = System::Drawing::Size(138, 23);
            this->btnProgExp->TabIndex = 4;
            this->btnProgExp->Text = L"Experimental Release";
            this->btnProgExp->UseVisualStyleBackColor = false;
            this->btnProgExp->Click += gcnew System::EventHandler(this, &Form1::btnProgExp_Click);
            // 
            // btnSave
            // 
            this->btnSave->BackColor = System::Drawing::Color::White;
            this->btnSave->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSave->Location = System::Drawing::Point(26, 25);
            this->btnSave->Name = L"btnSave";
            this->btnSave->Size = System::Drawing::Size(138, 23);
            this->btnSave->TabIndex = 5;
            this->btnSave->Text = L"Save Data";
            this->btnSave->UseVisualStyleBackColor = false;
            this->btnSave->Click += gcnew System::EventHandler(this, &Form1::btnSave_Click);
            // 
            // btnSetTime
            // 
            this->btnSetTime->BackColor = System::Drawing::Color::White;
            this->btnSetTime->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSetTime->Location = System::Drawing::Point(26, 83);
            this->btnSetTime->Name = L"btnSetTime";
            this->btnSetTime->Size = System::Drawing::Size(138, 23);
            this->btnSetTime->TabIndex = 6;
            this->btnSetTime->Text = L"Set Time";
            this->btnSetTime->UseVisualStyleBackColor = false;
            this->btnSetTime->Click += gcnew System::EventHandler(this, &Form1::btnSetTime_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(211, 8);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(87, 13);
            this->label1->TabIndex = 7;
            this->label1->Text = L"Update Firmware";
            // 
            // OnyxLink_version_label
            // 
            this->OnyxLink_version_label->AutoSize = true;
            this->OnyxLink_version_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular,
                System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
            this->OnyxLink_version_label->Location = System::Drawing::Point(0, 0);
            this->OnyxLink_version_label->Name = L"OnyxLink_version_label";
            this->OnyxLink_version_label->Size = System::Drawing::Size(52, 12);
            this->OnyxLink_version_label->TabIndex = 8;
            this->OnyxLink_version_label->Text = L"Version 1.1";
            // 
            // btnProgLocal
            // 
            this->btnProgLocal->BackColor = System::Drawing::Color::White;
            this->btnProgLocal->Enabled = false;
            this->btnProgLocal->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnProgLocal->Location = System::Drawing::Point(189, 112);
            this->btnProgLocal->Name = L"btnProgLocal";
            this->btnProgLocal->Size = System::Drawing::Size(138, 23);
            this->btnProgLocal->TabIndex = 9;
            this->btnProgLocal->Text = L"Local Firmware";
            this->btnProgLocal->UseVisualStyleBackColor = false;
            this->btnProgLocal->Click += gcnew System::EventHandler(this, &Form1::btnProgLocal_Click);
            // 
            // StatusLabel
            // 
            this->StatusLabel->AutoSize = true;
            this->StatusLabel->Location = System::Drawing::Point(43, 166);
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
            this->progressBar1->MarqueeAnimationSpeed = 0;
            this->progressBar1->Name = L"progressBar1";
            this->progressBar1->Size = System::Drawing::Size(352, 18);
            this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Marquee;
            this->progressBar1->TabIndex = 11;
            // 
            // btnTest
            // 
            this->btnTest->BackColor = System::Drawing::Color::White;
            this->btnTest->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnTest->Location = System::Drawing::Point(26, 112);
            this->btnTest->Name = L"btnTest";
            this->btnTest->Size = System::Drawing::Size(138, 23);
            this->btnTest->TabIndex = 12;
            this->btnTest->Text = L"Check Connection";
            this->btnTest->UseVisualStyleBackColor = false;
            this->btnTest->Click += gcnew System::EventHandler(this, &Form1::btnTest_Click);
            // 
            // zedGraphControl1
            // 
            this->zedGraphControl1->Location = System::Drawing::Point(12, 245);
            this->zedGraphControl1->Name = L"zedGraphControl1";
            this->zedGraphControl1->ScrollGrace = 0;
            this->zedGraphControl1->ScrollMaxX = 0;
            this->zedGraphControl1->ScrollMaxY = 0;
            this->zedGraphControl1->ScrollMaxY2 = 0;
            this->zedGraphControl1->ScrollMinX = 0;
            this->zedGraphControl1->ScrollMinY = 0;
            this->zedGraphControl1->ScrollMinY2 = 0;
            this->zedGraphControl1->Size = System::Drawing::Size(500, 205);
            this->zedGraphControl1->TabIndex = 13;
            // 
            // btnPlot
            // 
            this->btnPlot->BackColor = System::Drawing::Color::White;
            this->btnPlot->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnPlot->Location = System::Drawing::Point(26, 54);
            this->btnPlot->Name = L"btnPlot";
            this->btnPlot->Size = System::Drawing::Size(138, 23);
            this->btnPlot->TabIndex = 14;
            this->btnPlot->Text = L"Plot Data";
            this->btnPlot->UseVisualStyleBackColor = false;
            this->btnPlot->Click += gcnew System::EventHandler(this, &Form1::btnPlot_Data);
            // 
            // btnCloseGraph
            // 
            this->btnCloseGraph->BackColor = System::Drawing::Color::White;
            this->btnCloseGraph->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnCloseGraph->Location = System::Drawing::Point(528, 83);
            this->btnCloseGraph->Name = L"btnCloseGraph";
            this->btnCloseGraph->Size = System::Drawing::Size(119, 23);
            this->btnCloseGraph->TabIndex = 15;
            this->btnCloseGraph->Text = L"Close Graph";
            this->btnCloseGraph->UseVisualStyleBackColor = false;
            this->btnCloseGraph->Click += gcnew System::EventHandler(this, &Form1::btnClose_Graph);
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(77, 8);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(29, 13);
            this->label3->TabIndex = 16;
            this->label3->Text = L"Misc";
            // 
            // Form1
            // 
            this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
            this->BackColor = System::Drawing::Color::Azure;
            this->ClientSize = System::Drawing::Size(352, 193);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->btnCloseGraph);
            this->Controls->Add(this->btnPlot);
            this->Controls->Add(this->zedGraphControl1);
            this->Controls->Add(this->btnTest);
            this->Controls->Add(this->progressBar1);
            this->Controls->Add(this->StatusLabel);
            this->Controls->Add(this->btnProgLocal);
            this->Controls->Add(this->OnyxLink_version_label);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->btnSetTime);
            this->Controls->Add(this->btnSave);
            this->Controls->Add(this->btnProgExp);
            this->Controls->Add(this->btnProgBeta);
            this->Controls->Add(this->btnProgRelease);
            this->Controls->Add(this->btnClose);
            this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
            this->KeyPreview = true;
            this->Location = System::Drawing::Point(56, 48);
            this->MaximizeBox = false;
            this->MaximumSize = System::Drawing::Size(1500, 600);
            this->MinimumSize = System::Drawing::Size(303, 220);
            this->Name = L"Form1";
            this->Text = L"OnyxLink";
            this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
            this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::advanced_KeyDown);
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
            
            this->backgroundWorkerRunSaveData = gcnew System::ComponentModel::BackgroundWorker;
            backgroundWorkerRunSaveData->DoWork += gcnew DoWorkEventHandler(this, &Form1::backgroundWorkerRunSaveData_DoWork);
            backgroundWorkerRunSaveData->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &Form1::backgroundWorkerRunSaveData_RunWorkerCompleted);

            this->backgroundWorkerRunPlotData = gcnew System::ComponentModel::BackgroundWorker;
            backgroundWorkerRunPlotData->DoWork += gcnew DoWorkEventHandler(this, &Form1::backgroundWorkerRunPlotData_DoWork);
            backgroundWorkerRunPlotData->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &Form1::backgroundWorkerRunPlotData_RunWorkerCompleted);

        }

    private: System::Void btnSave_Click(System::Object ^  sender, System::EventArgs ^  e) {

        SaveFileDialog^ sfd = gcnew SaveFileDialog();
        sfd->Filter = "CSV Files|*.csv|All Files|*.*";
        sfd->AddExtension = true;

        System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;

        if( sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK ) {
            return;
        }

        UpdateUserMessage("Running, Acquiring and Plotting Data");
        StartProgressBar();
        backgroundWorkerRunSaveData->RunWorkerAsync(sfd->FileName);
    }

    private: System::Void backgroundWorkerRunSaveData_DoWork(Object^ sender, DoWorkEventArgs^ e) {
        BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
        System::String ^ filename = e->Argument->ToString();
        get_data_and_write_csv(filename);
        //why not plot the data that's just been saved
        plot_data(filename);
    }

    private: System::Void backgroundWorkerRunSaveData_RunWorkerCompleted(Object^ sender, RunWorkerCompletedEventArgs^ e) {
        UpdateUserMessage("Idle, Device Is Connected");
        StopProgressBar();
    }

    private: System::Void get_data_and_write_csv(String^ filename) {
        FileStream ^ fs = gcnew FileStream(filename, FileMode::Create);

        char *data = do_get_log_csv();

        for(int n=0;;n++) {
            fs->WriteByte(data[n]);
            if(data[n] == 0) break;
        }
        fs->Close();
        MessageBox::Show( "Save Complete" );
    }

    private: System::Void btnSetTime_Click(System::Object ^  sender, System::EventArgs ^  e) {
        do_set_time();
        MessageBox::Show( "Time Set" );
    }

    private: System::Void ButtonCancel_Click(System::Object ^  sender, System::EventArgs ^  e) {
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
        UpdateUserMessage("Running, Firmware Is Downloading, Do Not Disconnect");
        StartProgressBar();
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
        UpdateUserMessage("Running, Firmware Has Downloaded, Do Not Disconnect");
        StopProgressBar();
        //String^ firmware_version_to_flash = get_firmware_version_from_file( e->Result->ToString() );
        UpdateUserMessage("Running, Firmware Is Being Flashed, Do Not Disconnect");
        StartProgressBar();
        backgroundWorkerRunFlashLocal->RunWorkerAsync(e->Result->ToString());
    }

    private: System::Void runFlashLocal(char* szFileName) {
        UpdateUserMessage("Running, Firmware Is Being Flashed, Do Not Disconnect");
        StartProgressBar();
        char* firmware_version = get_firmware_version_from_file(szFileName);
        System::String^ s_firmware_version = char_star_to_system_string(firmware_version);

        System::String^ message_box_text = "Update firmware from " + m_cur_version_string + " to " + s_firmware_version;

        if( MessageBox::Show(message_box_text, 
                            "Firmware flash confirmation", 
                            MessageBoxButtons::OKCancel, 
                            MessageBoxIcon::Exclamation, 
                            MessageBoxDefaultButton::Button1) == ::System::Windows::Forms::DialogResult::Cancel ) {
            UpdateUserMessage("Idle, " + get_connection_status_string() );
            StopProgressBar();
            return;
        }

        UpdateUserMessage("Running, Firmware Being Flashed Is Version " + s_firmware_version);
        StartProgressBar();
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
        UpdateUserMessage("Idle, " + get_connection_status_string() );
        StopProgressBar();
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
        UpdateUserMessage("start running test");
        StartProgressBar();
        backgroundWorkerTest->RunWorkerAsync();
    }

    private: System::Void btnPlot_Data(System::Object ^ sender, System::EventArgs ^  e) {
        UpdateUserMessage("Running, Acquiring and Plotting Data");
        StartProgressBar();
        backgroundWorkerRunPlotData->RunWorkerAsync();
    }

    private: System::Void backgroundWorkerRunPlotData_DoWork(Object^ sender, DoWorkEventArgs^ e) {
        char szFileName[MAX_PATH];  
        TCHAR lpTempPathBuffer[MAX_PATH];
        DWORD dwRetVal = 0;
        UINT uRetVal = 0;

        // create temp file, the windows way
        // Gets the temp path env string (no guarantee it's a valid path).
        dwRetVal = GetTempPath(MAX_PATH, lpTempPathBuffer); 

        //  Generates a temporary file name. 
        uRetVal = GetTempFileName(lpTempPathBuffer, // directory for tmp files
                                  TEXT("DEMO"),     // temp file name prefix 
                                  0,                // create unique name 
                                  szFileName);      // buffer for name

        String^ filename = char_star_to_system_string(szFileName);
        get_data_and_write_csv(filename);
        plot_data(filename);
    }
                         
    private: System::Void backgroundWorkerRunPlotData_RunWorkerCompleted(Object^ sender, RunWorkerCompletedEventArgs^ e) {
        UpdateUserMessage("Idle, Device Is Connected");
        StopProgressBar();
    }

    private: System::Void btnClose_Graph(System::Object ^ sender, System::EventArgs ^  e) {
        this->Size = System::Drawing::Size(m_form_width, m_form_height);
    }

    private: System::Void plot_data(String^ filename) {
        create_graph(zedGraphControl1, filename);
        SetSize();
        this->Update();
    }

    private: System::Void SetSize() {
        
        //for restoring when closing graph
        m_form_width = this->Width;
        m_form_height = this->Height;

        int graph_start_x = 10;
        int graph_start_y = this->Height;

        zedGraphControl1->Location = System::Drawing::Point(graph_start_x, graph_start_y-30);

        int form_new_height = 600;
        int form_new_width = 1000;

        this->Size = System::Drawing::Size(form_new_width, form_new_height);

        int border = 25;
        // Leave a small margin around the outside of the control
        zedGraphControl1->Size = System::Drawing::Size(form_new_width-graph_start_x-border, form_new_height-graph_start_y-border);

    }

    private: System::Void backgroundWorkerTest_DoWork(Object^ sender, DoWorkEventArgs^ e) {
        // Get the BackgroundWorker that raised this event.
        BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);

        // Assign the result of the computation 
        // to the Result property of the DoWorkEventArgs 
        // object. This is will be available to the  
        // RunWorkerCompleted eventhandler.
        //e->Result = ComputeFibonacci(safe_cast<Int32>(e->Argument), worker, e);
        

        //Sleep(4000);
    }

    private: System::Void backgroundWorkerTest_RunWorkerCompleted(Object^ /*sender*/, RunWorkerCompletedEventArgs^ e ) {
        //char* char_version = do_get_version();
        //System::String^ version = char_star_to_system_string(char_version);
        //UpdateUserMessage(version, true);
        UpdateUserMessage(get_connection_status_string() );
        StopProgressBar();
    }

    private: System::Void UpdateUserMessage(System::String ^ message) {       
        this->StatusLabel->Text = message;
        this->Update();
    }

    private: System::Void StartProgressBar(void) {
        this->progressBar1->Style = ProgressBarStyle::Marquee;
        this->progressBar1->MarqueeAnimationSpeed = 30;
        this->Update();
    }

    private: System::Void StopProgressBar(void) {
        this->progressBar1->Style = ProgressBarStyle::Blocks;
        this->Update();
    }

    private: System::String^ get_connection_status_string() {
        bool device_is_connected = is_connected();
        System::String^ status = "no device found";

        if(device_is_connected) {
            char* cs_version = do_get_version();
            m_cur_version_string = char_star_to_system_string(cs_version);
            status = "device found and connected - " + m_cur_version_string;
        }
 
        return status;
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

    private: System::Void advanced_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {

        if(e->Control) {
            if(m_adv_features_enabled == false) {
                UpdateUserMessage("advanced features enabled");
                this->btnProgBeta->Enabled = true;
                this->btnProgExp->Enabled = true;
                this->btnProgLocal->Enabled = true;
                m_adv_features_enabled = true;
            }
            else {
                UpdateUserMessage("advanced features disabled");
                this->btnProgBeta->Enabled = false;
                this->btnProgExp->Enabled = false;
                this->btnProgLocal->Enabled = false;
                m_adv_features_enabled = false;
            }
            this->Update();
        }

    }


};
}


