#include "CScrollView.h"

CScrollView::CScrollView()
:m_fAdjustInterval(0.001f)
,m_bAutoAdjust(false)
,m_uRowsPerView(1)
,m_pScrollView(NULL)
,m_fLB(0.0f)
,m_fRB(0.0f)
{
}

CScrollView::~CScrollView()
{
}

CScrollView* CScrollView::create(CCNode* pContainer, CCSize& viewSize)
{
    CScrollView* pRet = new CScrollView();
    if ( pRet && pRet->init(pContainer, viewSize) )
    {
		pRet->setContentSize(pContainer->getContentSize());
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool CScrollView::init(CCNode* pContainer, CCSize& viewSize)
{
	bool ret = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		this->m_pScrollView = CCScrollView::create(viewSize, pContainer);
		this->m_pScrollView->setPosition(CCPointZero);
		this->m_pScrollView->setContentSize(pContainer->getContentSize());
		this->addChild(this->m_pScrollView);

		return true;
	}
	while(0);
	return ret;
}

void CScrollView::setDirection(const CCScrollViewDirection direction)
{
	this->m_pScrollView->setDirection(direction);
}
const CCScrollViewDirection CScrollView::getDirection()
{
	return this->m_pScrollView->getDirection();
}

void CScrollView::setScrollContentSize(const CCSize& size)
{
	this->m_pScrollView->setContentSize(size);
}
const CCSize& CScrollView::getScrollContentSize()
{
	return this->m_pScrollView->getContentSize();
}

void CScrollView::setBounceable(bool e)
{
	this->m_pScrollView->setBounceable(e);
}

const bool CScrollView::isBounceable()
{
	return this->m_pScrollView->isBounceable();
}

void CScrollView::setScrollDelegate(CCScrollViewDelegate* pDelegate)
{
	this->m_pScrollView->setDelegate(pDelegate);
}

CCScrollViewDelegate* CScrollView::getScrollDelegate()
{
	return this->m_pScrollView->getDelegate();
}

void CScrollView::setScrollViewSize(const CCSize& size)
{
	this->m_pScrollView->setViewSize(size);
}

CCSize CScrollView::getScrollViewSize()
{
	return this->m_pScrollView->getViewSize();
}

void CScrollView::onEnter()
{
	this->m_fLB = 0.0f;
	switch ( this->m_pScrollView->getDirection() )
	{
	case kCCScrollViewDirectionHorizontal: this->m_fRB = -this->m_pScrollView->getContentSize().width + this->m_pScrollView->getViewSize().width; break;
	case kCCScrollViewDirectionVertical: this->m_fRB = -this->m_pScrollView->getContentSize().height + this->m_pScrollView->getViewSize().height; break;
	default: this->m_fRB = 0.0f;
	}
	CCLayer::onEnter();

}

void CScrollView::scrollViewDidScroll(CCScrollView* view)
{
}

void CScrollView::scrollViewDidZoom(CCScrollView* view)
{
}

bool CScrollView::scrollViewAccelarateDidEnded(CCScrollView* view)
{
	if ( this->m_bAutoAdjust )
	{
		this->adjust(view);
	}
	return this->m_bAutoAdjust;
}

void CScrollView::setAutoAdjustEnabled(bool e)
{
	if ( this->m_bAutoAdjust != e )
	{
		this->m_bAutoAdjust = e;
		if ( this->m_pScrollView )
		{
		    this->adjust(this->m_pScrollView);
		}
	}
}

bool CScrollView::isAutoAdjustEnabled()
{
	return this->m_bAutoAdjust;
}

void CScrollView::adjust(CCScrollView* view)
{
	//�Ƿ���Ҫ�Զ�����
	if(this->m_bAutoAdjust)
	{
		//�ر�CCScrollView�е��Ե���
		//view->unscheduleAllSelectors();

		//�õ����ӷ�Χ�Ŀ��
		CCSize s = view->getViewSize();
		// ����λ��
		CCPoint adjustPos;
		// ��������ʱ��
		float adjustAnimDelay;
		switch ( view->getDirection() )
		{
		    case kCCScrollViewDirectionVertical:
			{
		    	float y = view->getContentOffset().y;
				float viewHeight = s.height/this->m_uRowsPerView;
				float offset = y - int(y/viewHeight)*viewHeight;
		    	if ( offset < -(viewHeight / 2) )
		    	{
		    		adjustPos = ccpSub(view->getContentOffset(), ccp(0, viewHeight + offset));
		    		adjustAnimDelay = (float)(viewHeight+offset)*this->m_fAdjustInterval;
		    	}
		    	else
		    	{
		            // ���㵱ǰҳλ�ã�ʱ��
		            adjustPos = ccpSub(view->getContentOffset(), ccp(0, offset));
		            // ����Ҫȡ����ֵ�������ڵ�һҳ���·�����ʱ����֤adjustAnimDelayΪ����
		    		adjustAnimDelay = (float) abs(offset) * this->m_fAdjustInterval;
		    	}
				if ( adjustPos.y < this->m_fRB )
				{
					adjustPos = ccp(view->getContentOffset().x, this->m_fRB);
				}
				else if ( adjustPos.y > this->m_fLB )
				{
					adjustPos = ccp(view->getContentOffset().x, this->m_fLB);
				}
				view->setContentOffset(adjustPos, true);
		        break;
			}
		    case kCCScrollViewDirectionHorizontal:
		    {
		    	float x = view->getContentOffset().x;
				float viewWidth = s.width/this->m_uRowsPerView;
				float offset = x - int(x/viewWidth)*viewWidth;
		    	if ( offset < -(viewWidth / 2) )
		    	{
		            adjustPos = ccpSub(view->getContentOffset(), ccp(viewWidth+offset, 0));

		    		adjustAnimDelay = (float)(viewWidth+offset)*this->m_fAdjustInterval;
		    	}
		    	else
		    	{
		            // ���㵱ǰҳλ�ã�ʱ��
		            adjustPos = ccpSub(view->getContentOffset(), ccp(offset, 0));

		            // ����Ҫȡ����ֵ�������ڵ�һҳ���󷭶���ʱ����֤adjustAnimDelayΪ����
		    		adjustAnimDelay = (float) abs(offset) * this->m_fAdjustInterval;
		    	}
				if ( adjustPos.y < this->m_fRB )
				{
					adjustPos = ccp(view->getContentOffset().x, this->m_fRB);
				}
				else if ( adjustPos.y > this->m_fLB )
				{
					adjustPos = ccp(this->m_fLB, view->getContentOffset().y);
				}
				view->setContentOffset(adjustPos, true);
		        break;
		    }
		    default:
		    	break;
		}
	}
}
