#include "Telemetry.h"

/*
#include <windows.h>
#include <Vcclr.h>
#include <wininet.h>
*/

#include <Windows.h>
#include <WinInet.h>
#pragma comment (lib, "Wininet.lib")
#pragma comment (lib, "urlmon.lib")
#define POST 1
#define GET 0

#define MAX_MESSAGE_LENGTH 1024


#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;


#include <msclr/marshal.h>
using namespace msclr::interop;

#include <iostream>
#include <string>

using namespace std;


Telemetry::Telemetry(const char* version, const char* guid) : m_ip("176.58.121.212") {

    mp_json_metrics_doc = new Document;
    const unsigned IBUFSIZE = 1024;

    // Parse an empty JSON objext to initialize the document.
    char json[IBUFSIZE] = "{  }";
    if (mp_json_metrics_doc->Parse<0>(json).HasParseError()) {
        // Oops, something went wrong
    }
    else {
        Value myVal;

        // This is just a string that is added to the document.
        myVal.SetString("onyx_firmware_loader_metrics", mp_json_metrics_doc->GetAllocator());
        mp_json_metrics_doc->AddMember("document_name", myVal, mp_json_metrics_doc->GetAllocator());

        // Now we add a new object to the document that will be used to store its own info
        // First we add the member. Then we define th new member as an object.
        // Then we get a reference to the object so that we can more easily add to
        // or read from it.
        const char* newValue = "";
        mp_json_metrics_doc->AddMember("parameters", newValue, mp_json_metrics_doc->GetAllocator());
        (*mp_json_metrics_doc)["parameters"].SetObject();
        mp_outParams = &(*mp_json_metrics_doc)["parameters"];

        // Now we add a name/value pair to the object outParams instead of the
        // document mp_json_metrics_doc.
        
        //myVal.SetInt(17);
        mp_outParams->AddMember("version", version, mp_json_metrics_doc->GetAllocator());
        mp_outParams->AddMember("guid", guid, mp_json_metrics_doc->GetAllocator());

    }
}

Telemetry::~Telemetry() {
}

//send once off messages along with tracking info
bool Telemetry::send_metrics_pair(const char* key, const char* value) {

    if( mp_outParams->HasMember(key) ) {
        mp_outParams->RemoveMember(key);
    }
    mp_outParams->AddMember(key, value, mp_json_metrics_doc->GetAllocator());

    bool result = send_metrics();

    mp_outParams->RemoveMember(key);

    return result;
}


bool Telemetry::add_or_update_json_key_value(const char* key, const char* value) {
    if (key && value) {
        if(mp_outParams->HasMember(key) ) {
            mp_outParams->RemoveMember(key);
        }
        mp_outParams->AddMember(key, value, mp_json_metrics_doc->GetAllocator());
        return true;
    }
    return false;
}

bool Telemetry::send_metrics(void) {

    StringBuffer strbuf;
    Writer<StringBuffer> writer(strbuf);
    mp_json_metrics_doc->Accept(writer);
    const char* metric_data = strbuf.GetString();
    std::cout << metric_data << std::endl;

    const char* url = "/cgi/parse_json_get.py?data=";
    char char_url_data[MAX_MESSAGE_LENGTH];
    strcpy(char_url_data, url); // copy string one into the result.
    strcat(char_url_data, metric_data); // append string two to the result.

    //was planning on posting json use this array but cann't get it to work so far
    char unused_data[1024] = "";
    return request(GET, m_ip, char_url_data, "Content-Type: text/plain", unused_data);
}


bool Telemetry::request(int Method, char* Host, char* url, char* header, char* data)
{
    bool sent_successfully = false;

    try {
        //Retrieve default http user agent
        char httpUseragent[512];
        DWORD szhttpUserAgent = sizeof(httpUseragent);
        ObtainUserAgentString(0, httpUseragent, &szhttpUserAgent);

        char m[5];

        if (Method == GET)
            strcpy_s(m, 5, "GET\0");
        else
            strcpy_s(m, 5, "POST\0");

        HINTERNET internet = InternetOpenA(httpUseragent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (internet != NULL) {
            HINTERNET connect = InternetConnectA(internet, Host, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
            if (connect != NULL) {
                HINTERNET request = HttpOpenRequestA(connect, m, url, "HTTP/1.1", NULL, NULL,
                    INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
                    INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
                    INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
                    INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
                    INTERNET_FLAG_NO_AUTH |
                    INTERNET_FLAG_NO_CACHE_WRITE |
                    INTERNET_FLAG_NO_UI |
                    INTERNET_FLAG_PRAGMA_NOCACHE |
                    INTERNET_FLAG_RELOAD, NULL);

                if (request != NULL) {

                    int datalen = 0;
                    if (data != NULL) datalen = strlen(data);
                    int headerlen = 0;
                    if (header != NULL) headerlen = strlen(header);

                    sent_successfully = HttpSendRequestA(request, header, headerlen, data, datalen);

                    InternetCloseHandle(request);
                }
            }
            InternetCloseHandle(connect);
        }
        InternetCloseHandle(internet);
    }
    catch (...) {}

    return sent_successfully;

}

























