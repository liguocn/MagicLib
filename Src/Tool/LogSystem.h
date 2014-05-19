#pragma once
#include <fstream>
#include <iostream>

namespace MagicTool
{
    enum LogLevel
    {
        LOGLEVEL_DEBUG = 0,
        LOGLEVEL_INFO,
        LOGLEVEL_WARN,
        LOGLEVEL_ERROR,
        LOGLEVEL_OFF
    };

    extern const LogLevel gSystemLogLevel;

#define MagicLog(level) \
    if (level < MagicTool::gSystemLogLevel) ;\
    else MagicTool::LogSystem::GetSingleton()->GetOFStream() 
#define DebugLog MagicLog(MagicTool::LOGLEVEL_DEBUG)
#define InfoLog MagicLog(MagicTool::LOGLEVEL_INFO)
#define WarnLog MagicLog(MagicTool::LOGLEVEL_WARN)
#define ErrorLog MagicLog(MagicTool::LOGLEVEL_ERROR)

    class LogSystem
    {
    private:
        static LogSystem* mpLogSystem;
        LogSystem(void);
    public:
        static LogSystem* GetSingleton(void);
        ~LogSystem(void);
        std::ofstream& GetOFStream();
    public:
        std::ofstream mOFStream;
    };
}
