#include "UITextEdit.h"
#include "Util.h"
#include <algorithm>
#include <numeric>


static const char* _ALLOWPASSWORD = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$_-";
//#define IME_SIZE SDL_min(IME_WAIT_SIZE, m_ims.size())
#define LINE_HEIGHT 20

UITextEdit::UITextEdit()
	: UIControl()
{
	m_size = { 75,25 };
}

void UITextEdit::Update(uint32_t dt)
{
	m_ticks++;
	if (m_ticks % 50 == 0)
		cursor.visiable = !cursor.visiable;
}

void UITextEdit::Render()
{
	auto rc = GetGlobalRect();
	{
		sRender->FillRect(rc, m_backgroundColor);
	}

	if (IsTextSelected())
	{
		auto sr = getSelectionRect();
		SDL_Color sc{ 38, 79, 120, 80 };
		sRender->FillRect(sr, sc);
	}

	if (!m_text.empty())
	{
		sRender->DrawString(getRectText().c_str(), m_fontSize, rc);
	}

	if (!IsPassword() && !editing.text.empty())
	{
		if (m_textoffset > 0)
		{
			std::wstring wstr = m_wtext.substr(m_textoffset);
			auto [w, h] = sRender->GetTextSize(wstr.c_str(), m_fontSize);
			sRender->DrawString(editing.text.c_str(), m_fontSize, SDL_Point{ rc.x + w + 4, rc.y });
		}
		else
			sRender->DrawString(editing.text.c_str(), m_fontSize, SDL_Point{ rc.x + m_textwidth + 4, rc.y });
	}

	if (IsFocused() && !IsTextSelected())
	{
		if (cursor.visiable)
		{
			SDL_Rect rccc = calcCursorRect(cursor.position);
			sRender->FillRect(rccc, SDL_Color{ 0,0,0,255 });
		}
	}
}

bool UITextEdit::Handle(SDL_Event* e)
{
	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		int mx = e->button.x;
		if (e->button.button == SDL_BUTTON_LEFT)
		{
			clearSelection();
			setCursorPos(mx);
			selection.pos1 = cursor.position;
			selection.start = cursor.position;
			//SDL_StartTextInput();
		}
	}
	else if (e->type == SDL_MOUSEMOTION && SDL_GetMouseState(0, 0) == SDL_BUTTON_LEFT)
	{
		int mx = e->motion.x;
		setCursorPos(mx);
		selection.end = cursor.position;
		selection.pos2 = cursor.position;

		if (selection.pos1 > 0 || selection.pos2 > 0)
		{
			selection.start = SDL_min(selection.pos1, selection.pos2);
			selection.end = SDL_max(selection.pos1, selection.pos2);
		}
	}
	else if (e->type == SDL_TEXTINPUT)
	{
		if (m_wtext.size() >= GetLength()) return true;
		if (!IsReadOnly())
		{
			if (!IsPassword())
			{
				std::string str = GetText();
				if (!IsTextSelected())
				{
					str += e->text.text;
					auto [w, h] = sRender->GetTextSize(str.c_str(), m_fontSize);
					m_textwidth = w;
					m_textheight = h;
					std::wstring ww = utf8_to_wstring(e->text.text);
					if (m_numonly)
					{
						if (!isAllDigits(ww)) return UIControl::Handle(e);
					}
					cursor.position += (int)ww.length();
					SetText(str);
					editing.text = "";
					editing.width = 0;
					editing.height = 0;
					editing.index = 0;
					m_ims.clear();
				}
				else
				{
					replaceSelectionText(e->text.text);
				}
			}
			else
			{
				std::string str = GetText();
				if (strstr(_ALLOWPASSWORD, e->text.text))
				{
					str += e->text.text;
					cursor.position += (int)strlen(e->text.text);
					SetText(str);
				}
			}
		}
	}
	else if (e->type == SDL_TEXTEDITING)
	{
		if (m_wtext.size() >= GetLength()) return true;
		if (!IsReadOnly())
		{
			auto wid = sRender->GetWindowHandle();
			editing.text = e->edit.text;
			auto [w, h] = sRender->GetTextSize(editing.text.c_str(), m_fontSize);
			editing.width = w;
			editing.height = h;
			if (editing.index == -1)
				editing.index = 0;

			/*m_ims = GetCandidateList(wid);
			editing.index = GetCurrentCandidateIndex(wid);
			if (editing.index >= (int)IME_SIZE)
			{
				editing.index = (int)IME_SIZE - 1;
				SelectCandidate(wid, editing.index);
			}*/
		}
	}
	else if (e->type == SDL_KEYDOWN)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_BACKSPACE:
		{
			if (!IsReadOnly())
			{
				if (IsTextSelected())
				{
					removeSelectedText();
				}
				else if (cursor.position > 0)
				{
					cursor.position--;
					removeCursorCharater();
				}

				auto [w, h] = sRender->GetTextSize(m_text.c_str(), m_fontSize);
				m_textwidth = w;
				m_textheight = h;
			}
		}
		break;
		case SDLK_DELETE:
		{
			if (!IsReadOnly())
			{
				if (IsTextSelected())
				{
					removeSelectedText();
				}
				else if (cursor.position < m_wtext.length())
				{
					removeCursorCharater();
				}
			}
			//fprintf(stderr, "textoffset: %d, curpos: %d\n", m_textoffset, cursor.position);
		}
		break;
		case SDLK_LEFT:
		{
			if (m_textoffset == cursor.position && m_textoffset > 0)
				m_textoffset--;

			if (cursor.position > 0)
				cursor.position--;

			//fprintf(stderr, "textoffset: %d, curpos: %d\n", m_textoffset, cursor.position);
		}
		break;
		case SDLK_RIGHT:
		{
			if (isCursorRight())
			{
				if (cursor.position < (int)m_wtext.length())
				{
					m_textoffset++;
					cursor.position++;
				}
			}
			else
			{
				if (cursor.position < (int)m_wtext.length())
					cursor.position++;
			}
			//fprintf(stderr, "textoffset: %d, curpos: %d\n", m_textoffset, cursor.position);
		}
		break;
		case SDLK_HOME:
		{
			cursor.position = 0;
			m_textoffset = 0;
		}
		break;
		case SDLK_END:
		{
			cursor.position = (int)m_wtext.length();
			calcTextOffset();
		}
		break;
		}
	}
	return UIControl::Handle(e);
}

void UITextEdit::OnTextChanged(const std::string& old)
{
	m_wtext = utf8_to_wstring(m_text);
	calcCharacterWidths();
}

void UITextEdit::OnMouseOver()
{
	SDL_Cursor* cur = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	SDL_SetCursor(cur);
	UIControl::OnMouseOver();
}

void UITextEdit::OnMouseOut()
{
	SDL_Cursor* cur = SDL_GetDefaultCursor();
	SDL_SetCursor(cur);
	UIControl::OnMouseOver();
}

void UITextEdit::OnFocusChanged()
{
	if (!IsFocused())
	{
		SDL_StopTextInput();
		m_ims.clear();
	}
	else
	{
		SDL_StartTextInput();
	}
}

std::string UITextEdit::GetSelectionText() const
{
	if (IsPassword())
	{
		auto data = getRectText();
		std::string str = std::string(&data[selection.start], selection.end - selection.start);
		return str;
	}
	else
	{
		std::wstring str = std::wstring(&m_wtext[selection.start], selection.end - selection.start);
		return wstring_to_utf8(str);
	}
}

void UITextEdit::SetPassowrd(bool _password)
{
	if (m_ispassword != _password)
	{
		m_ispassword = _password;
		calcCharacterWidths();
	}
}

void UITextEdit::calcCharacterWidths()
{
	m_chw.clear();
	if (IsPassword())
	{
		std::string utf8 = getDisplayText();
		auto [w, h] = sRender->GetTextSize(utf8.c_str(), m_fontSize);
		for (size_t i = 0; i < utf8.length(); i++)
			m_chw.push_back(w / (int)utf8.length());
	}
	else
	{
		for (int i = 0; i < m_wtext.length(); i++)
		{
			auto [w, h] = sRender->GetTextSize(std::wstring(1, m_wtext[i]).c_str(), m_fontSize);
			m_chw.push_back(w);
		}
	}

	calcTextOffset();
}

void UITextEdit::SelectAll()
{
	selection.start = 0;
	selection.end = (int)m_wtext.length();
}

void UITextEdit::calcTextOffset()
{
	if (m_textoffset == cursor.position) return;
	auto rc = GetGlobalRect();
	int w = int(m_size.x) - 2;
	for (int i = m_chw.size() - 1; i >= 0; i--)
	{
		w -= m_chw[i];
		if (w < 0)
			break;
		m_textoffset = i;
	}
}

SDL_Rect UITextEdit::getSelectionRect()
{
	auto rc = GetGlobalRect();
	SDL_Rect rs;
	{
		int ow = std::accumulate(m_chw.begin(), m_chw.begin() + m_textoffset, 0);
		std::string utf8 = getDisplayText();
		std::string str = std::string(&utf8[selection.start], selection.end - selection.start);
		auto [w, h] = sRender->GetTextSize(str.c_str(), m_fontSize);
		int w1 = 0, h1 = 0;
		if (selection.start > 0)
		{
			str = std::string(&utf8[0], selection.start);
			auto [cw, ch] = sRender->GetTextSize(str.c_str(), m_fontSize);
			w1 = cw;
			h1 = ch;
		}
		rs = { rc.x + 4 + w1 - ow, rc.y + (int(m_size.y) - h) / 2, w, h};
	}
	return rs;
}

void UITextEdit::setCursorPos(int x)
{
	auto rc = GetGlobalRect();
	x -= rc.x;
	if (x <= 0)
	{
		cursor.position = 0;
		return;
	}

	for (int i = m_textoffset; i < m_chw.size(); i++)
	{
		x -= m_chw[i];
		if (x < 0)
		{
			if (SDL_abs(x) < m_chw[i] / 2)
				cursor.position = i + 1;
			else
				cursor.position = i;
			break;
		}
		else if (x == 0)
		{
			cursor.position = i;
			break;
		}
	}

	if (x > 0)
		cursor.position = (int)m_chw.size();
}

void UITextEdit::clearSelection()
{
	selection.start = selection.end = 0;
}

SDL_Rect UITextEdit::calcCursorRect(int pos)
{
	auto rc = GetGlobalRect();
	rc.x += 2;
	for (int i = m_textoffset; i < m_chw.size(); i++)
	{
		if (i < pos)
			rc.x += m_chw[i];
	}

	if (editing.text.empty())
		return SDL_Rect{ rc.x - 1, rc.y + 1, 2, LINE_HEIGHT - 2 };
	else
		return SDL_Rect{ rc.x - 1 + editing.width, rc.y + 1, 2, LINE_HEIGHT - 2 };
}

//SDL_Rect UITextEdit::calcImeListRect()
//{
//	auto rc = calcCursorRect(cursor.position);
//	return calcImeListRect(rc);
//}

//SDL_Rect UITextEdit::calcImeListRect(const UIRect& rc)
//{
//	static int width = 60;
//	int th = 0, he = 0;
//	int imesize = (int)(IME_SIZE);
//	auto& ws = m_ims[0];
//	auto [w, h] = sRender->GetTextSize(wstring_to_utf8(ws).c_str(), m_fontSize);
//	he = h;
//	th = imesize * he;
//	return { rc.x, rc.y + rc.height, width, th };
//}

void UITextEdit::removeCursorCharater()
{
	m_wtext.erase(m_wtext.begin() + cursor.position);
	m_text = wstring_to_utf8(m_wtext);
	calcCharacterWidths();
}

void UITextEdit::removeSelectedText()
{
	cursor.position = selection.start;
	m_wtext.erase(selection.start, selection.end - selection.start);
	selection.start = selection.end = 0;
	m_text = wstring_to_utf8(m_wtext);
	calcCharacterWidths();
}

void UITextEdit::replaceSelectionText(const std::string& _utf8)
{
	std::wstring wstr = utf8_to_wstring(_utf8);
	cursor.position = selection.start;
	m_wtext.erase(selection.start, selection.end - selection.start);
	m_wtext.insert(cursor.position, wstr.c_str());
	selection.start = selection.end = 0;
	m_text = wstring_to_utf8(m_wtext);
	cursor.position += (int)wstr.length();
	calcCharacterWidths();
}

std::string UITextEdit::getRectText() const
{
	int w = (int)m_size.x;
	int wc = 0;
	if (IsPassword())
	{
		wc = (w - 2) / m_chw[0];
		std::string str(SDL_min((int)(m_wtext.size() - m_textoffset), wc), '*');
		return str;
	}
	else
	{
		//w -= 2;
		for (int i = m_textoffset; i < m_wtext.size(); i++)
		{
			w -= m_chw[i];
			if (w < 0)
				break;
			wc++;
		}
		std::wstring str = m_wtext.substr(m_textoffset, wc);
		//fwprintf_s(stderr, L"%s\n", str.c_str());
		return wstring_to_utf8(str);
	}
}

std::string UITextEdit::getDisplayText() const
{
	if (IsPassword())
	{
		std::string str(m_wtext.size(), '*');
		return str;
	}
	else
	{
		return wstring_to_utf8(m_wtext);
	}
}

bool UITextEdit::isCursorRight() const
{
	int w = (int)m_size.x;
	if (m_wtext.size() == cursor.position) return true;
	int cw = std::accumulate(m_chw.begin() + m_textoffset, m_chw.begin() + cursor.position + 1, 0);
	if (cw > w)
		return true;
	return false;
}

bool UITextEdit::isAllDigits(const std::string& str)
{
	return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

bool UITextEdit::isAllDigits(const std::wstring& str)
{
	return !str.empty() && std::all_of(str.begin(), str.end(), ::iswdigit);
}
