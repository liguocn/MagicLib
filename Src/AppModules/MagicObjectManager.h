#pragma once
#include "MagicObject.h"
#include <map>
#include <string>

namespace MagicApp
{
#define MOMGR MagicApp::MagicObjectManager::GetSingleton() 

    class MagicObjectManager
    {
    private:
        static MagicObjectManager* mpMOMgr;
        MagicObjectManager();
        
    public:
        static MagicObjectManager* GetSingleton(void);
        ~MagicObjectManager();

        MagicObject* GetObj(const std::string& objName);
        bool IsObjExist(const std::string& objName);
        void InsertObj(const std::string& objName, MagicObject* pObj);
        
    public:
        std::map<std::string, MagicObject*> mObjectMap;
    };
}

