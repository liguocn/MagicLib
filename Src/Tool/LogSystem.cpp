#include "../Tool/LogSystem.h"

namespace MagicTool
{
    const LogLevel gSystemLogLevel = LOGLEVEL_DEBUG;
    LogSystem* LogSystem::mpLogSystem = NULL;

    LogSystem::LogSystem(void)
        : mOFStream("Magic3D.log")
    {
    }

    LogSystem* LogSystem::GetSingleton()
    {
        if (mpLogSystem == NULL)
        {
            mpLogSystem = new LogSystem;
        }
        return mpLogSystem;
    }

    LogSystem::~LogSystem(void)
    {
    }

    std::ofstream& LogSystem::GetOFStream()
    {
        return mOFStream;
    }

    bool gLogOut = false;
}

