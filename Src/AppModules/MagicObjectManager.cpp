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
}
