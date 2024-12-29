#include "bulb.hpp"

#include <jansson.h>
#include <stdio.h>

// According to random sources online, the port that the bulb uses to broadcast
// is 38899
constexpr int WIZ_UDP_BROADCAST_PORT {38899};

Bulb::Bulb()
{
    mPort = WIZ_UDP_BROADCAST_PORT;
}

Bulb::~Bulb()
{

}

void Bulb::SetDeviceIP(const std::string& ip)
{ 
    mDevIP = ip;
}


std::string Bulb::GetDeviceIP()
{
    return mDevIP;
}

std::string Bulb::Discover(const std::string& ip)
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getDevInfo"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    printf("discover request %s to wiz", msg.c_str());
    std::string devIP = "yes";
    auto resp = mSocket.SendUdpCommand(msg, ip, mPort, devIP);
    return ParseResponse(resp, devIP);
}

std::string Bulb::ParseResponse(std::string jsonStr, std::string addlParams)
{
    if (jsonStr.empty())
        return jsonStr;

    json_t* root = json_object();
    json_error_t error;
    json_t* data = json_loads(jsonStr.c_str(), 0, &error);
    if (!data) {
        printf("JSON parse on [%d]: %s", error.line, error.text);
        return "";
    }

    if (!json_is_object(data)) {
        printf("JSON parsing error\nData is not an object");
        return "";
    }

    json_t* res = json_object_get(data, "result");
    if (!res || !json_is_object(res)) {
        printf("JSON parsing error\nResult is not an object");
        return "";
    }

    json_t* dataObj = json_object();
    json_t* val;
    for (void* itr = json_object_iter(res); itr != NULL; itr = json_object_iter_next(res, itr)) {
        const char* key = json_object_iter_key(itr);
        val = json_object_iter_value(itr);
        json_object_set(dataObj, key, val);
    }

    json_object_del(dataObj, "method");
    json_object_del(dataObj, "id");
    json_object_del(dataObj, "env");

    if (!addlParams.empty())
        json_object_set_new(dataObj, "ip", json_string(addlParams.c_str()));

    json_object_set_new(root, "bulb_response", dataObj);
    std::string output = json_dumps(root, JSON_INDENT(4));
    printf("%s", output.c_str());
    return output;
}
