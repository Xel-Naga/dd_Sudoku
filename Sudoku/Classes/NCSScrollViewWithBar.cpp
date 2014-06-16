#include "NCSScrollViewWithBar.h"

NCSScrollViewWithBar::NCSScrollViewWithBar()
:m_pSlideSlot(NULL)
,m_pSlideBar(NULL)
,m_oPreviousContentOffset(CCPointZero)
,m_oSlideBarRect(CCRectMake(0,0,0,0))
,m_fBarBottom(0.0f)
,m_fBarTop(0.0f)
,m_fScrollViewBottom(0.0f)
,m_fScrollViewTop(0.0f)
,m_fRatio(0.0f)
,m_oViewSize(CCSizeMake(0,0))
{
}

NCSScrollViewWithBar::~NCSScrollViewWithBar()
{
}
NCSScrollViewWithBar* NCSScrollViewWithBar::create(CCNode* pContainer, CCSize& viewSize, const char* slotFrameName, const char* barFrameName)
{
    NCSScrollViewWithBar* pRet = new NCSScrollViewWithBar();
    if ( pRet && pRet->init(pContainer, viewSize, slotFrameName, barFrameName) )
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
};

bool NCSScrollViewWithBar::init(CCNode* pContainer, CCSize& viewSize, const char* slotFrameName, const char* barFrameName)
{
    bool ret = false;

    do
    {
		CC_BREAK_IF(!CScrollView::init(pContainer, viewSize) || NULL == pContainer);

        this->m_pScrollView->setPosition(ccp(0,0));
		this->m_pScrollView->setDelegate(this);
		this->setAutoAdjustEnabled(true);
		this->m_pScrollView->setDirection(kCCScrollViewDirectionVertical);
		this->m_pScrollView->setBounceable(true);
        pContainer->setPosition(ccp(0, -(pContainer->getContentSize().height - viewSize.height)));
        this->m_oPreviousContentOffset = pContainer->getPosition();
        this->m_oViewSize = viewSize;
        this->m_fScrollViewTop = this->m_oPreviousContentOffset.y;
        this->m_fScrollViewBottom = 0.0f;

        this->m_fRatio = viewSize.height / this->m_pScrollView->getContentSize().height;

        this->m_pSlideSlot = CCScale9Sprite::createWithSpriteFrameName(slotFrameName);
		float slotWidth = this->m_pSlideSlot->getContentSize().width;
		float slotHeight = viewSize.height;
		this->m_pSlideSlot->setContentSize(CCSizeMake(slotWidth, slotHeight));

        float x = viewSize.width + 20 + slotWidth/2;
        float y = viewSize.height/2;
        this->m_pSlideSlot->setPosition(ccp(x,y));
        this->addChild(this->m_pSlideSlot);

        this->m_pSlideBar = CCScale9Sprite::createWithSpriteFrameName(barFrameName);
        y = slotHeight * ( 1 - this->m_fRatio/2 );
        CCSize s = this->m_pSlideBar->getContentSize();
        CCSize realS = CCSizeMake(s.width, slotHeight*this->m_fRatio);
        this->m_pSlideBar->setContentSize(realS);
        this->m_pSlideBar->setPosition(ccp(x,y));
		this->addChild(this->m_pSlideBar);

        this->m_fBarBottom = realS.height/2;
        this->m_fBarTop    = viewSize.height - this->m_fBarBottom;
        this->m_oSlideBarRect = CCRectMake(-realS.width/2, -realS.height/2, realS.width, realS.height);

		ret = true;
    }
	while (0);

	return ret;
}

void NCSScrollViewWithBar::onEnter()
{
	this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchPriority(0);
	CScrollView::onEnter();
	this->setTouchEnabled(true);
}

bool NCSScrollViewWithBar::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	if ( !this->isVisible() )
	{
		return false;
	}

	CCPoint p = this->m_pSlideBar->convertTouchToNodeSpaceAR(pTouch);
	if ( this->m_oSlideBarRect.containsPoint(p) )
    {
        return true;
    }

    return false;
}

void NCSScrollViewWithBar::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	CCPoint contentOffset = this->m_pScrollView->getContentOffset();
	CCPoint newContentOffset = ccp(contentOffset.x,0);
    CCPoint currPosition = pTouch->getLocation();
    CCPoint prePosition = pTouch->getPreviousLocation();    
	float deltaYInPercent = (currPosition.y - prePosition.y)/this->m_oViewSize.height;
	if ( contentOffset.y >= this->m_fScrollViewBottom && deltaYInPercent < 0 )
	{
		newContentOffset.y = this->m_fScrollViewBottom;
	}
	else if ( contentOffset.y <= this->m_fScrollViewTop && deltaYInPercent > 0)
	{
		newContentOffset.y = this->m_fScrollViewTop;
	}
	else
	{
        CCPoint contentOffset = this->m_pScrollView->getContentOffset();
        newContentOffset = ccp(contentOffset.x,
                               contentOffset.y - deltaYInPercent * this->m_pScrollView->getContentSize().height);
	}
    this->m_pScrollView->setContentOffset(newContentOffset);
}

void NCSScrollViewWithBar::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    this->ccTouchMoved(pTouch, pEvent);
}

void NCSScrollViewWithBar::setVisible(bool e)
{
	if ( this->isVisible() != e )
	{
		CCLayer::setVisible(e);
		//设置scrollview的可见性，防止当scrollview不可见时，它的ccTouchBegan错误的响应
		this->m_pScrollView->setVisible(e);
	}
}

void NCSScrollViewWithBar::scrollViewDidScroll(CCScrollView* view)
{
    CCPoint currentContentOffset = view->getContentOffset();
	if ( currentContentOffset.y > this->m_fScrollViewBottom || currentContentOffset.y < this->m_fScrollViewTop )
	{
		return;
	}
    float deltaY = ccpSub(currentContentOffset, this->m_oPreviousContentOffset).y;
    float deltaYInPercent = deltaY / view->getContainer()->getContentSize().height;
	
    CCPoint p = this->m_pSlideBar->getPosition();
	CCPoint newP = ccp(p.x, p.y - deltaYInPercent * this->m_oViewSize.height);
	if ( newP.y > this->m_fBarTop )//
	{
		newP.y = this->m_fBarTop;
	}
	else if ( newP.y < this->m_fBarBottom )
	{
		newP.y = this->m_fBarBottom;
	}
    this->m_pSlideBar->setPosition(newP);
	this->m_oPreviousContentOffset = currentContentOffset;
}