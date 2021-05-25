#include "qlSettings.hpp"
#include "quickldr.h"
#include <GDMake.h>
#include <thread>

using namespace gdmake;

std::vector<qlInstruction*> qlSettings::getInstructionsForLoadTime(qlLoadTime time) {
    return this->instructions[time];
}

void qlSettings::executeForLoadTime(qlLoadTime time) {
    if (this->instructions.find(time) == this->instructions.end())
        return;

    for (auto v : this->instructions[time])
        if (v != nullptr) {
            if (dynamic_cast<qlDLL*>(v))
                LoadLibraryA((dynamic_cast<qlDLL*>(v))->path.c_str());
            else if (dynamic_cast<qlWait*>(v))
                std::this_thread::sleep_for(std::chrono::milliseconds(dynamic_cast<qlWait*>(v)->waitTime));
        }
}

qlInstruction* qlSettings::parseInstruction(std::string const& str) {
    qlInstruction* res = nullptr;

    if (str.find(".dll") != std::string::npos)
        res = new qlDLL(std::string(folderName) + "/" + str);
    if (str._Starts_with("wait") && str.find_first_of(' ') != std::string::npos)
        res = new qlWait(std::stoi(str.substr(4)));
    
    if (res != nullptr)
        res->instruction = str;

    return res;
}

bool qlSettings::load(std::string const& text) {
    std::string t = text;
    qlLoadTime curr = qlLoadTime::qlStartUp;

    this->instructions[qlLoadTime::qlMenuScreen] = std::vector<qlInstruction*> ();
    this->instructions[qlLoadTime::qlSplashScreen] = std::vector<qlInstruction*> ();
    this->instructions[qlLoadTime::qlStartUp] = std::vector<qlInstruction*> ();

    while (t.length() > 0) {
        std::string ins;
        if (t.find_first_of('\n') != std::string::npos) {
            ins = t.substr(0, t.find_first_of('\n'));

            t = t.substr(t.find_first_of('\n') + 1);
        } else {
            ins = t;
        
            t = "";
        }

        if (ins[ins.length() - 1] == '\r')
            ins = ins.substr(0, ins.length() - 1);

        auto lt = loadTimeForStr(ins.c_str());

        if (lt != qlLoadTime::qlUnknown)
            curr = lt;
        else {
            auto it = this->instructions.find(curr);

            if (it != this->instructions.end())
                it->second.push_back(this->parseInstruction(ins));
        }
    }
    
    return true;
}

qlSettings::~qlSettings() {
    for (auto [_, m] : this->instructions)
        for (auto v : m)
            if (v != nullptr)
                delete v;
}
