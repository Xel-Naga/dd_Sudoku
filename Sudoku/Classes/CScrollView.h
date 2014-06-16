#ifndef __C_SCROLL_VIEW_H__
#define __C_SCROLL_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class CScrollView : public CCLayer, public CCScrollViewDelegate
{
    CC_SYNTHESIZE(float, m_fAdjustInterval, AdjustInterval);
	CC_SYNTHESIZE(CCScrollView*, m_pScrollView, ScrollView);
	//when horizontal, means columns; when vertical, means rows
	CC_SYNTHESIZE(unsigned int, m_uRowsPerView, RowsPerView);
    public:
        CScrollView();
        virtual ~CScrollView();

    public:
        static CScrollView* create(CCNode* pContainer, CCSize& viewSize);
		virtual bool init(CCNode* pContainer, CCSize& viewSize);

    public:
		virtual void onEnter();

		void setDirection(const CCScrollViewDirection direction);
		const CCScrollViewDirection getDirection();

		void setScrollContentSize(const CCSize& size);
		const CCSize& getScrollContentSize();

		void setBounceable(bool e);
		const bool isBounceable();

		void setScrollDelegate(CCScrollViewDelegate* pDelegate);
		CCScrollViewDelegate* getScrollDelegate();

		void setScrollViewSize(const CCSize& size);
		CCSize getScrollViewSize();

		//实现代理接口
		virtual void scrollViewDidScroll(CCScrollView* view);
		virtual void scrollViewDidZoom(CCScrollView* view);
		virtual bool scrollViewAccelarateDidEnded(CCScrollView* view);

        void setAutoAdjustEnabled(bool e);
        bool isAutoAdjustEnabled();
        void adjust(CCScrollView* view);

    protected:
        bool m_bAutoAdjust;
		float m_fLB, m_fRB;
};


#endif // __C_SCROLL_VIEW_H__
