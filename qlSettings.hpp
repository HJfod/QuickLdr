#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

enum qlLoadTime {
    qlUnknown,
    qlStartUp,
    qlSplashScreen,
    qlMenuScreen,
};

inline qlLoadTime loadTimeForStr(const char* str) {
    if (strcmp(str, "Startup") == 0)
        return qlLoadTime::qlStartUp;
    if (strcmp(str, "Splash") == 0)
        return qlLoadTime::qlSplashScreen;
    if (strcmp(str, "Menu") == 0)
        return qlLoadTime::qlMenuScreen;

    return qlLoadTime::qlUnknown;
}

struct qlInstruction {
    std::string instruction;
    virtual inline void dummyShit() {}
};

struct qlDLL : public qlInstruction {
    std::string path;

    inline qlDLL(std::string const& p) {
        this->path = p;
    }
};

struct qlWait : public qlInstruction {
    unsigned int waitTime;

    inline qlWait(unsigned int wt) {
        this->waitTime = wt;
    }
};

class qlSettings {
    protected:
        std::unordered_map<qlLoadTime, std::vector<qlInstruction*>> instructions;

        qlInstruction* parseInstruction(std::string const&);

    public:
        std::vector<qlInstruction*> getInstructionsForLoadTime(qlLoadTime t);
        void executeForLoadTime(qlLoadTime t);

        bool load(std::string const&);

        ~qlSettings();
};
