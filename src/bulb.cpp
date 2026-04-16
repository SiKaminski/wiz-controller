#include "bulb.hpp"
#include "globals.hpp"

#include <jansson.h>

namespace Wiz::Bulb
{
    std::string EMPTY_STRING;

    Device::Device(Meta meta) : mMeta(meta)
    {
        mSocket = UDP::Socket();
        mState = State{};
    }

    Device::~Device()
    {

    }

    void Device::SetDeviceIP(const std::string& ip)
    { 
        mMeta.Ip = ip.c_str();
    }


    std::string Device::GetDeviceIP()
    {
        return mMeta.Ip;
    }

    std::string Device::Discover(const std::string& ip)
    {
        json_t* root = json_object();
        json_object_set_new(root, "method", json_string("getDevInfo"));

        std::string msg = json_dumps(root, JSON_COMPACT);
        Global::logger.Log(INFO, "discover request %s to wiz\n", msg.c_str());
        std::string devIP = "yes";
        auto resp = mSocket.SendUdpCommand(msg, ip, mMeta.Port, devIP);
        return ParseResponse(resp, devIP);
    }

    std::string Device::GetStatus()
    {
        return "";
    }

    std::string Device::GetDeviceInfo()
    {
        return "";
    }

    std::string Device::GetWifiConfig()
    {
        return "";
    }

    std::string Device::GetSystemConfig()
    {
        return "";
    }

    std::string Device::GetUserConfig()
    {
        return "";
    }

    std::string Device::ToggleLight(bool state)
    {
        json_t* root = json_object();
        json_object_set_new(root, "id", json_integer(1));
        json_object_set_new(root, "method", json_string("setState"));

        json_t* data = json_object();
        json_object_set_new(data, "state", json_boolean(state));
        json_object_set_new(root, "params", data);

        std::string msg = json_dumps(root, JSON_COMPACT);
        Global::logger.Log(INFO, "Light turning [%s]\n", (state ? "ON" : "OFF"));
        Global::logger.Log(INFO, "toggleLight request %s to wiz", msg.c_str());
        auto resp = mSocket.SendUdpCommand(msg, mMeta.Ip, mMeta.Port, EMPTY_STRING);
        return ParseResponse(resp);
    }

    std::string Device::SetBrightness(int brightness)
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
        Global::logger.Log(INFO, "toggleLight request %s to wiz", msg.c_str());
        auto resp = mSocket.SendUdpCommand(msg, mMeta.Ip, mMeta.Port, EMPTY_STRING);
        return ParseResponse(resp);
    }

    std::string Device::SetRGB(Color color)
    {
        // Check if rgb values are in bound
        if (!color.Valid())
            return Global::ERR_BRIGHTNESS_OUT_OF_BOUNDS;

        json_t* root = json_object();
        json_object_set_new(root, "id", json_integer(1));
        json_object_set_new(root, "method", json_string("setPilot"));

        json_t* data = json_object();
        json_object_set_new(data, "r", json_integer(color.R));
        json_object_set_new(data, "g", json_integer(color.G));
        json_object_set_new(data, "b", json_integer(color.B));
        json_object_set_new(root, "params", data);

        std::string msg = json_dumps(root, JSON_COMPACT);
        Global::logger.Log(INFO, "Change color request %s to wiz", msg.c_str());
        auto resp = mSocket.SendUdpCommand(msg, mMeta.Ip, mMeta.Port, EMPTY_STRING);
        return ParseResponse(resp);
    }

    std::string Device::ParseResponse(std::string jsonStr, std::string addlParams)
    {
        if (jsonStr.empty())
            return jsonStr;

        json_t* root = json_object();
        json_error_t error;
        json_t* data = json_loads(jsonStr.c_str(), 0, &error);
        if (!data) {
            Global::logger.Log(INFO, "JSON parse on [%d]: %s\n", error.line, error.text);
            return "";
        }

        if (!json_is_object(data)) {
            Global::logger.Log(INFO, "JSON parsing error\nData is not an object\n");
            return "";
        }

        json_t* res = json_object_get(data, "result");
        if (!res || !json_is_object(res)) {
            Global::logger.Log(INFO, "JSON parsing error\nResult is not an object\n");
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
        
        Global::logger.Log(INFO, "%s\n", output.c_str());
        return output;
    }
} // namespace Wiz::Bulb
