#pragma once

//#define PLATFORM_WIN
#define PLATFORM_MAC

namespace MagicTool
{
    class Profiler
    {
    public:
        Profiler();
        ~Profiler();

        static double GetTime(void);

    };
}
