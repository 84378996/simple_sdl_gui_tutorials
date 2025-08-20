#pragma once
#include "UIControl.h"
class UIScrollView : public UIControl
{
	enum
	{
		UP,
		DOWN,
		BAR,
		BK
	};

public:
	UIScrollView();
	~UIScrollView() = default;

	void Render() override;

public:
	void SetContentHeight(int _sch);
	virtual void OnSize(int cx, int cy) override;
	virtual bool Handle(SDL_Event* e) override;
	virtual void OnMouseOut() override;
	virtual void InnerScrollVisiableChanged(bool _visiable) {};
	bool IsScrollbarVisiabled() const { return m_scrollVisiable; }
	void SetAlwaysShowScrollBar(bool _show) { m_alwaysShowScrollbar = _show; }
	virtual void OnMove(int x, int y) override;
private:
	void CalcScrollSize(bool cnt2bar = true);
	int OnHitTest(int x, int y);

protected:
	void OnScrollUp(int step = 1);
	void OnScrollDown(int step = 1);
	static const int _BAR_SIZE{ 20 };
	static const int _PART_COUNT{ 4 };
	int m_viewheight{ 0 };
	int m_lineheight{ 0 };
	int m_linespace{ 0 };
	int m_linecount{ 0 };
	int m_contentheight{ 0 };
	bool m_scrollVisiable{ false };
	bool m_dragenable{ false };
	int m_dragStart{ 0 };
	SDL_Rect m_rects[4]{};
	int m_scrollOffset{ 0 }; //当前滚动偏移量，表示已滚动的像素。
	bool m_alwaysShowScrollbar{ false };

private:
	int m_maxScrollOffset{ 0 }; //最大滚动偏移量，maxScrollOffset = contentHeight - viewHeight。
	int m_thumbTrackHeight{ 0 }; //滑块可滑动的轨道高度，thumbTrackHeight = viewHeight - thumbHeight。
	int m_thumbHeight{ 0 }; //滑块高度
	int m_thumbPosition{ 0 }; //滑块位置
};
