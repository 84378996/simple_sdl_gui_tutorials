#include "UIScrollView.h"

UIScrollView::UIScrollView()
	: UIControl()
{
	m_lineheight = TTF_FontHeight(sRender->GetFont(m_fontSize));
}

void UIScrollView::Render()
{
	SDL_Color sc[_PART_COUNT] = { {150, 150, 150, 255},{150, 150, 150, 255},{100, 100, 255, 255} , {200, 200, 200, 255} };
	SDL_Rect rc = GetGlobalRect();
	sRender->FillRect(rc, SDL_Color{ 255,255,255,255 });

	if (m_scrollVisiable)
	{
		for (int i = _PART_COUNT - 1; i >= 0; --i)
			sRender->FillRect(m_rects[i], sc[i]);
	}
}

void UIScrollView::SetContentHeight(int _sch)
{
	m_linecount = _sch;
	OnSize(m_size.x, m_size.y);
}


void UIScrollView::OnSize(int cx, int cy)
{
	bool v = m_scrollVisiable;
	m_scrollVisiable = (m_lineheight + m_linespace) * m_linecount > cy;
	//if (m_scrollVisiable)
	{
		CalcScrollSize();
	}

	if (v != m_scrollVisiable)
	{
		InnerScrollVisiableChanged(m_scrollVisiable);
	}
}

bool UIScrollView::Handle(SDL_Event* e)
{
	if (e->type == SDL_MOUSEWHEEL)
	{
		if (m_scrollVisiable)
		{
			if (e->wheel.y > 0) //up
			{
				OnScrollUp(3);
			}
			if (e->wheel.y < 0)
			{
				OnScrollDown(3);
			}
		}
		return true;
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		if (e->button.button == SDL_BUTTON_LEFT)
		{
			if (m_scrollVisiable)
			{
				int ret = OnHitTest(e->button.x, e->button.y);
				if (ret == UP)
				{
					OnScrollUp();
				}
				else if (ret == DOWN)
				{
					OnScrollDown();
				}
				else if (ret == BK)
				{
					if (e->button.y < m_rects[BAR].y)
					{
						OnScrollUp(5);
					}
					else
					{
						OnScrollDown(5);
					}
				}
				else if (ret == BAR)
				{
					m_dragenable = true;
					m_dragStart = e->button.y;
				}
			}
		}
	}
	else if (e->type == SDL_MOUSEBUTTONUP)
	{
		m_dragenable = false;
	}
	else if (e->type == SDL_MOUSEMOTION)
	{
		if (m_dragenable)
		{
			if (m_scrollOffset < m_maxScrollOffset && m_scrollOffset >0)
			{
				m_thumbPosition += (e->motion.y - m_dragStart);
				CalcScrollSize(false);
				m_dragStart = e->motion.y;
			}
			else if ((m_scrollOffset == m_maxScrollOffset && e->motion.y < m_dragStart)
				|| (m_scrollOffset == 0 && e->motion.y > m_dragStart))
			{
				m_thumbPosition += (e->motion.y - m_dragStart);
				CalcScrollSize(false);
				m_dragStart = e->motion.y;
			}
		}
	}
	return UIControl::Handle(e);
}

void UIScrollView::OnMouseOut()
{
	if (m_dragenable)
		m_dragenable = false;
	__super::OnMouseOut();
}

void UIScrollView::OnMove(int x, int y)
{
	__super::OnMove(x, y);
}

void UIScrollView::CalcScrollSize(bool cnt2bar)
{
	Vec2 sz = GetSize();
	int cx = sz.x;
	int cy = sz.y;

	m_viewheight = sz.y - 2 * _BAR_SIZE;
	m_contentheight = m_linecount * (m_lineheight + m_linespace);

	m_thumbHeight = m_viewheight * 1.0f *m_size.y / m_contentheight;
	m_maxScrollOffset = m_contentheight - sz.y;
	m_thumbTrackHeight = m_viewheight - m_thumbHeight;
	if (cnt2bar)
		m_thumbPosition = static_cast<int>((float)m_scrollOffset / m_maxScrollOffset * m_thumbTrackHeight);
	else
		m_scrollOffset = static_cast<int>((float)m_thumbPosition / m_thumbTrackHeight * m_maxScrollOffset);

	if (m_scrollOffset > m_maxScrollOffset)
		m_scrollOffset = m_maxScrollOffset;
	if (m_scrollOffset < 0)
		m_scrollOffset = 0;

	auto r = GetGlobalRect();
	m_rects[0] = { r.x + r.w - _BAR_SIZE, r.y, _BAR_SIZE, _BAR_SIZE };
	m_rects[1] = { r.x + r.w - _BAR_SIZE, r.y + r.h - _BAR_SIZE, _BAR_SIZE, _BAR_SIZE };
	m_rects[2] = { r.x + r.w - _BAR_SIZE, r.y + _BAR_SIZE + m_thumbPosition, _BAR_SIZE, m_thumbHeight };
	m_rects[3] = { r.x + r.w - _BAR_SIZE, r.y, _BAR_SIZE, r.h };
}

void UIScrollView::OnScrollUp(int step)
{
	int prepos = m_scrollOffset;
	m_scrollOffset -= step * (m_lineheight + m_linespace);
	if (m_scrollOffset < 0)
		m_scrollOffset = 0;

	CalcScrollSize();
}

void UIScrollView::OnScrollDown(int step)
{
	int prepos = m_scrollOffset;
	m_scrollOffset += step * (m_lineheight + m_linespace);
	if (m_scrollOffset > m_maxScrollOffset)
		m_scrollOffset = m_maxScrollOffset;
	CalcScrollSize();
}

int UIScrollView::OnHitTest(int x, int y)
{
	if (!m_scrollVisiable) return -1;
	for (int i = 0; i < _PART_COUNT; i++)
	{
		SDL_Point pt{ x,y };
		if (SDL_PointInRect(&pt, &m_rects[i])) return i;
	}
	return -1;
}