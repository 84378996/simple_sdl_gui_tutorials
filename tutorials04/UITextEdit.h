#pragma once
#include "UIControl.h"

class UITextEdit : public UIControl
{
public:
	UITextEdit();
	~UITextEdit() = default;

	void Update(uint32_t dt) override;
	void Render() override;
	bool Handle(SDL_Event* e) override;
	void OnTextChanged(const std::string& old) override;
	void OnMouseOver() override;
	void OnMouseOut() override;
	void OnFocusChanged() override;
	std::string GetSelectionText() const;
	bool IsReadOnly() const { return m_isreadonly; }
	inline void SetReadOnly(bool _readonly) { m_isreadonly = _readonly; }

	bool IsPassword() const { return m_ispassword; }
	void SetPassowrd(bool _password);

	inline void SetLength(int len) { m_length = len; }
	inline int GetLength() const { return m_length; }
	void SelectAll();
	void SetNumOnly(bool _numOnly = true) { m_numonly = _numOnly; }
protected:
	void calcTextOffset();
	void calcCharacterWidths();
	SDL_Rect getSelectionRect();
	void setCursorPos(int x);
	void clearSelection();
	SDL_Rect calcCursorRect(int pos);
	//SDL_Rect calcImeListRect();
	//SDL_Rect calcImeListRect(const SDL_Rect& rcCursor);
	void removeCursorCharater();
	void removeSelectedText();
	void replaceSelectionText(const std::string& _utf8);
	std::string getRectText() const;
	std::string getDisplayText() const;
	bool isCursorRight() const;
	bool isAllDigits(const std::string& str);
	bool isAllDigits(const std::wstring& str);
protected:
	struct
	{
		bool visiable{ false };
		int position{ 0 };
	} cursor;

	struct
	{
		int start{ 0 };
		int end{ 0 };
		int pos1{ 0 };
		int pos2{ 2 };
	}selection;
	struct
	{
		std::string text;
		int width;
		int height;
		int index{ -1 };
	}editing;

	bool IsTextSelected() const { return selection.end - selection.start > 0; }

	std::wstring m_wtext;

	std::vector<int> m_chw;
	bool m_isreadonly{ false };
	bool m_ispassword{ false };
	bool m_numonly{ false };
	std::vector<std::wstring> m_ims;
	uint32_t m_ticks{ 0 };
	int m_textwidth{ 0 };
	int m_textheight{ 0 };
	int m_textoffset{ 0 };
	int m_length{ 32 };
	const int IME_WAIT_SIZE = 8;
};