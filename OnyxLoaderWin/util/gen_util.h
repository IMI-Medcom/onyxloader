#ifndef UTIL_H_
#define UTIL_H_

#include <Vcclr.h>
#include <wininet.h>
#include <msclr/marshal.h>
using namespace msclr::interop;


System::String ^ char_star_to_system_string(const char * in) {
    System::String^ out;
    out = gcnew System::String(in);
    return out;
}

#endif /* UTIL_H_ */