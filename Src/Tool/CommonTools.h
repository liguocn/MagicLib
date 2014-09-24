#pragma once
#include <string>

namespace MagicTool
{
    class CommonTools
    {
    public:
        CommonTools();
        ~CommonTools();

        static std::string GetPath(const std::string& fileName);

    };
}
