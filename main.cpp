// generated through GDMake https://github.com/HJfod/GDMake

// include GDMake & submodules
#include <GDMake.h>
#include <filesystem>
#include "quickldr.h"
#include "qlSettings.hpp"

void  __declspec(dllexport) zstub(){}

qlSettings* sett;

using namespace gdmake;

GDMAKE_HOOK(0x18c080)
bool __fastcall LoadingLayer_init(unknown_t self, edx_t edx, bool idk) {
    if (!GDMAKE_ORIG(self, edx, idk))
        return false;
    
    if (!idk)
        sett->executeForLoadTime(qlLoadTime::qlSplashScreen);

    return true;
}

GDMAKE_HOOK(0x1907b0)
bool __fastcall MenuLayer_init(unknown_t self) {
    if (!GDMAKE_ORIG(self))
        return false;

    if (sett != nullptr) {
        sett->executeForLoadTime(qlLoadTime::qlMenuScreen);

        delete sett;

        sett = nullptr;
    }

    return true;
}

GDMAKE_MAIN {
    std::string settPath = std::string(folderName) + "/" + settingsFile;

    sett = new qlSettings();

    if (!std::filesystem::exists(folderName))
        std::filesystem::create_directory(folderName);

    if (std::filesystem::exists(settPath))
        sett->load(gdmake::readFileString(settPath));
    else
        gdmake::writeFileString(settPath, "");

    sett->executeForLoadTime(qlLoadTime::qlStartUp);

    return true;
}

GDMAKE_UNLOAD {}
