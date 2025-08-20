#pragma once
#include "UIControl.h"
#include "UIScrollView.h"

class UITextField : public UIScrollView
{
	struct TLine
	{
		SDL_Color c{ 0,0,0,255 };
		std::wstring txt;

		bool isLink{ false };
		std::wstring tag;
		std::wstring value;
		SDL_Rect rect{};
		bool hover{ false };
	};

public:
	UITextField();
	UITextField(const char* _tex);
	~UITextField() = default;

	void Render() override;
	bool Handle(SDL_Event* e) override;

	void OnTextChanged(const std::string& oldText) override;
	void UpdateScrollBarPos();
	void OnMove(int x, int y) override;
	void SetLinkClickedCallback(std::function<void(uint32_t, uint32_t)> _linkclickedfun) { m_linkclickedfun = _linkclickedfun; }

private:
	void ResolveText();
	void InnerScrollVisiableChanged(bool _visiable) override;

private:
	std::vector<std::vector<TLine>> m_items;
	Vec2 m_ptHit;
	Vec2 m_ptThum;
	int m_offsetHit{ 0 };
	int m_linespace{ 5 };
	std::wstring m_wtext;
	std::vector<int> m_textsize;
	std::function<void(uint32_t, uint32_t)> m_linkclickedfun;
};