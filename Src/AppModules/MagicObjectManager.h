#pragma once
#include "MagicObject.h"
#include <map>
#include <string>

namespace MagicApp
{
    class MagicObjectManager
    {
    private:
        static MagicObjectManager* mpMOMgr;
        MagicObjectManager();
        
    public:
        static MagicObjectManager* GetSingleton(void);
        ~MagicObjectManager();
        
    public:
        std::map<std::string, MagicObject*> mObjectMap;
    };
}

