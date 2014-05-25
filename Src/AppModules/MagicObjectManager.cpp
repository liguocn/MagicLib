#include "MagicObjectManager.h"

namespace MagicApp
{
    MagicObjectManager* MagicObjectManager::mpMOMgr = NULL;
    
    MagicObjectManager::MagicObjectManager()
    {
    }
    
    MagicObjectManager::~MagicObjectManager()
    {
    }
    
    MagicObjectManager* MagicObjectManager::GetSingleton()
    {
        if (mpMOMgr == NULL)
        {
            mpMOMgr = new MagicObjectManager;
        }
        return mpMOMgr;
    }

    MagicObject* MagicObjectManager::GetObj(const std::string& objName)
    {
        std::map<std::string, MagicObject* >::iterator itr = mObjectMap.find(objName);
        if (itr != mObjectMap.end())
        {
            return itr->second;
        }
        else
        {
            return NULL;
        }
    }

    bool MagicObjectManager::IsObjExist(const std::string& objName)
    {
        std::map<std::string, MagicObject* >::iterator itr = mObjectMap.find(objName);
        if (itr != mObjectMap.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void MagicObjectManager::InsertObj(const std::string& objName, MagicObject* pObj)
    {
        std::map<std::string, MagicObject* >::iterator itr = mObjectMap.find(objName);
        if (itr != mObjectMap.end())
        {
            delete itr->second;
            itr->second = NULL;
        }
        mObjectMap[objName] = pObj;
    }
}
