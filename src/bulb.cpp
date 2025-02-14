#include "bulb.hpp"

#include <jansson.h>
#include <stdio.h>

// According to random sources online, the port that the bulb uses to broadcast
// is 38899
constexpr int WIZ_UDP_BROADCAST_PORT {38899};

std::string EMPTY_STRING;

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
    printf("discover request %s to wiz\n", msg.c_str());
    std::string devIP = "yes";
    auto resp = mSocket.SendUdpCommand(msg, ip, mPort, devIP);
    return ParseResponse(resp, devIP);
}

std::string Bulb::GetStatus()
{

}

std::string Bulb::GetDeviceInfo()
{

}

std::string Bulb::GetWifiConfig()
{

}

std::string Bulb::GetSystemConfig()
{

}

std::string Bulb::GetUserConfig()
{

}

std::string Bulb::ToggleLight(bool state)
{
    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setState"));

    json_t* data = json_object();
    json_object_set_new(data, "state", json_boolean(state));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    printf("Light turning [%s]\n", (state ? "ON" : "OFF"));
    printf("toggleLight request %s to wiz", msg.c_str());
    auto resp = mSocket.SendUdpCommand(msg, mDevIP, mPort, EMPTY_STRING);
    return ParseResponse(resp);
}

std::string Bulb::SetBrightness(int brightness)
{
    // Brightness is out of bounds
    if (brightness < 0 || brightness > 100)
        return "Invalid";

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setPilot"));

    json_t* data = json_object();
    json_object_set_new(data, "dimming", json_integer(brightness));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    printf("toggleLight request %s to wiz", msg.c_str());
    auto resp = mSocket.SendUdpCommand(msg, mDevIP, mPort, EMPTY_STRING);
    return ParseResponse(resp);
}

std::string Bulb::SetRGB(ushort r, ushort g, ushort b)
{
    // Check if rgb values are in bound
    if (!ColorInBound(r) || !ColorInBound(g) || !ColorInBound(b))
        return "Invalid";

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setPilot"));

    json_t* data = json_object();
    json_object_set_new(data, "r", json_integer(r));
    json_object_set_new(data, "g", json_integer(g));
    json_object_set_new(data, "b", json_integer(b));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    printf("Change color request %s to wiz", msg.c_str());
    auto resp = mSocket.SendUdpCommand(msg, mDevIP, mPort, EMPTY_STRING);
    return ParseResponse(resp);
}

bool Bulb::ColorInBound(ushort val)
{
    return val >= 0 && val <= 255;
}

std::string Bulb::ParseResponse(std::string jsonStr, std::string addlParams)
{
    if (jsonStr.empty())
        return jsonStr;

    json_t* root = json_object();
    json_error_t error;
    json_t* data = json_loads(jsonStr.c_str(), 0, &error);
    if (!data) {
        printf("JSON parse on [%d]: %s\n", error.line, error.text);
        return "";
    }

    if (!json_is_object(data)) {
        printf("JSON parsing error\nData is not an object\n");
        return "";
    }

    json_t* res = json_object_get(data, "result");
    if (!res || !json_is_object(res)) {
        printf("JSON parsing error\nResult is not an object\n");
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
    printf("%s\n", output.c_str());
    return output;
}
