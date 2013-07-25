#ifndef _LEVELLAYERLOADER_H_
#define _LEVELLAYERLOADER_H_

#include "LevelLayer.h"

/* Forward declaration. */
class CCBReader;

namespace XAXA {
class LevelLayerLoader : public cocos2d::extension::CCLayerLoader {
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LevelLayerLoader, loader);

    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LevelLayer);
};
}

#endif
