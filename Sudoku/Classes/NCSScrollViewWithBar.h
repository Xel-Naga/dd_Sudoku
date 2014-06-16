#ifndef __NCS_SCROLL_VIEW_WITH_BAR_H__
#define __NCS_SCROLL_VIEW_WITH_BAR_H__

#include "CScrollView.h"
USING_NS_CC;

class NCSScrollViewWithBar : public CScrollView
{
    public:
        NCSScrollViewWithBar();
        virtual ~NCSScrollViewWithBar();

    public:
        static NCSScrollViewWithBar* create(CCNode* pContainer, CCSize& viewSize, const char* slotFrameName, const char* barFrameName);
        virtual bool init(CCNode* pContainer, CCSize& viewSize, const char* slotFrameName, const char* barFrameName);

        virtual void onEnter();

        virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
        virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
        virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

		virtual void setVisible(bool e);
        //实现代理接口
        virtual void scrollViewDidScroll(CCScrollView* view);

    private:
        CCScale9Sprite* m_pSlideSlot;
        CCScale9Sprite* m_pSlideBar;
        CCPoint         m_oPreviousContentOffset;
		CCRect          m_oSlideBarRect;
		float           m_fBarBottom, m_fBarTop;
        float           m_fScrollViewBottom, m_fScrollViewTop;
		float           m_fRatio;
        CCSize          m_oViewSize;
		
};

#endif // __NCS_SCROLL_VIEW_WITH_BAR_H__
