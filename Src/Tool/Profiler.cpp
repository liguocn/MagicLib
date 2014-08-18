#include "Profiler.h"
#ifdef PLATFORM_WIN
#include <windows.h>
#endif

namespace MagicTool
{
    Profiler::Profiler()
    {
    }

    Profiler::~Profiler()
    {
    }

    double Profiler::GetTime()
    {
        double curTime = 0;
#ifdef PLATFORM_WIN
        static __int64 start = 0;
        static __int64 frequency = 0;

        if (start == 0)
        {
            QueryPerformanceCounter((LARGE_INTEGER*)&start);
            QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
            return 0.0f;
        }

        __int64 counter = 0;
        QueryPerformanceCounter((LARGE_INTEGER*)&counter);
        curTime = (counter - start) / double(frequency);
#endif
        return curTime;
    }

}
