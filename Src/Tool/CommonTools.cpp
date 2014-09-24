#include "CommonTools.h"

namespace MagicTool
{
    CommonTools::CommonTools()
    {
    }

    CommonTools::~CommonTools()
    {
    }

    std::string CommonTools::GetPath(const std::string& fileName)
    {
        std::string path = fileName;
        std::string::size_type pos = path.rfind("/");
        if (pos == std::string::npos)
        {
            pos = path.rfind("\\");
        }
        path.erase(pos);
        
        return path;
    }
}
