#pragma once

#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;

class Telemetry
{
public:
    Telemetry(const char* version, const char* guid);
    ~Telemetry();

    bool send_metrics_pair(const char* key, const char* value);
    bool send_metrics(void);

    bool add_or_update_json_key_value(const char* name, const char* value);

private:

    Document* mp_json_metrics_doc;

    bool transmit_message(System::String^ message);
    bool request(int Method, char* Host, char* url, char* header, char* data);

    char* m_ip;

    Value* mp_outParams;

};

