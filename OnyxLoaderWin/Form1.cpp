//#include "stdafx.h"
#include "Form1.h"
#include <windows.h>


using namespace TryUSB;


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
    Application::EnableVisualStyles();
    Application::Run(gcnew Form1());
    return 0;
}
