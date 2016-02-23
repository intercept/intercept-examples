#include <Windows.h>
#include <stdio.h>
#include <cstdint>
#include <atomic>

#include "intercept.hpp"
#include "logging.hpp"
#include "client\client.hpp"
#include "client\pointers.hpp"
#include "client\sqf\uncategorized.hpp"

INITIALIZE_EASYLOGGINGPP

int __cdecl intercept::api_version() {
    return 1;
}

void __cdecl intercept::on_frame() {

}

void __cdecl intercept::post_init() {

}

void __cdecl intercept::mission_stopped() {

}

void __cdecl intercept::fired(
    object &unit_,
    rv_string &weapon_,
    rv_string &muzzle_,
    rv_string &mode_,
    rv_string &ammo_,
    rv_string &magazine,
    object &projectile_) 
{

}


void Init(void) {
    el::Configurations conf;

    conf.setGlobally(el::ConfigurationType::Filename, "logs/intercept_aps_demo.log");
    conf.setGlobally(el::ConfigurationType::MaxLogFileSize, "10240");
#ifdef _DEBUG
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "[%datetime] - %level - {%loc}t:%thread- %msg");
    conf.setGlobally(el::ConfigurationType::PerformanceTracking, "true");
#else
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime-{%level}- %msg");
#endif
    el::Loggers::setDefaultConfigurations(conf, true);

    LOG(INFO) << "Intercept APS Demo DLL Loaded";
}

void Cleanup(void) {

}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Init();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        Cleanup();
        break;
    }
    return TRUE;
}
