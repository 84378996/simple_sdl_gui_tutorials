#include "UITextFeild.h"
#include <regex>
#include "UIColor.h"
#include "Util.h"
#include <unordered_map>

UITextField::UITextField()
    : UITextField(nullptr)
{
    m_linespace = 5;
}

UITextField::UITextField(const char* txt)
    : UIScrollView()
{
    if (txt == nullptr)
        SetText("");
    else
        SetText(txt);
}

void UITextField::Render()
{
    __super::Render();

    if (m_textsize.size() <= 1) return;
    int h = m_textsize[1];
    auto rcClient = GetGlobalRect();
    {
        SDL_Rect _rc = rcClient;
        SDL_RenderSetClipRect(sRender->GetRender(), &_rc);
    }

    int startItem = (m_scrollOffset % (m_lineheight + m_linespace)) ? (m_scrollOffset / (m_lineheight + m_linespace) + 1) : (m_scrollOffset / (m_lineheight + m_linespace));
    int endItem = SDL_min(startItem + m_size.y / (m_lineheight + m_linespace), m_items.size());

    // 确保 endItem 不会超出范围
    if (endItem > m_items.size()) {
        endItem = m_items.size();
    }

    int yPos = rcClient.y + m_lineheight / 2;
    // Draw the items
    for (int i = startItem; i < endItem; ++i)
    {
        int startX = rcClient.x + 5;
        for (auto& segment : m_items[i]) {
            // 将 std::wstring 转换为 UTF-8
            std::wstring utf8Text = segment.txt;
            auto [ww, wh] = sRender->GetTextSize(utf8Text.c_str(), m_fontSize);

            SDL_Rect destRect = { startX, yPos, ww, wh };
            //SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
            sRender->DrawString(utf8Text.c_str(), m_fontSize, destRect, segment.c);
            segment.rect = destRect;
            if (segment.hover && segment.isLink)
                sRender->DrawLine(startX, yPos + wh, startX + ww, yPos + wh, Color::GREEN);

            // 更新 x 位置
            startX += ww;
        }

        // 更新 y 位置到下一行
        yPos += m_lineheight + m_linespace;
    }
    auto rcWnd = sRender->GetClientRect();
    SDL_RenderSetClipRect(sRender->GetRender(), &rcWnd);
}

bool UITextField::Handle(SDL_Event* e)
{
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
    {
        SDL_Point pt{ e->button.x, e->button.y };
        for (auto& segvec : m_items)
        {
            for (auto& seg : segvec)
            {
                if (SDL_PointInRect(&pt, &seg.rect))
                {
                    if (seg.isLink)
                    {
                        if (m_linkclickedfun)
                            m_linkclickedfun(_wtoi(seg.tag.c_str()), _wtoi(seg.value.c_str()));
                        break;
                    }
                }
            }
        }
    }
    else if (e->type == SDL_MOUSEMOTION)
    {
        SDL_Point pt{ e->motion.x, e->motion.y };
        for (auto& segvec : m_items)
        {
            for (auto& seg : segvec)
            {
                if (seg.isLink)
                    seg.hover = (SDL_PointInRect(&pt, &seg.rect));
            }
        }
    }
    return __super::Handle(e);
}

void UITextField::OnTextChanged(const std::string& oldText)
{
    ResolveText();
}

void UITextField::UpdateScrollBarPos()
{
    InnerScrollVisiableChanged(false);
}

void UITextField::OnMove(int x, int y)
{
    __super::OnMove(x, y);
    SetContentHeight((int)m_items.size());
}


void UITextField::ResolveText()
{
    m_items.clear();
    //m_links.clear();
    //m_hoverLink = nullptr;

    if (m_text.empty()) {
        m_wtext.clear();
        return;
    }

    m_wtext = utf8_to_wstring(m_text);
    m_textsize = sRender->GetTextWidths(m_wtext, m_fontSize);

    Vec2 sz = GetSize();
    bool hasScroll = IsScrollbarVisiabled();
    int lineWidth = sz.x - 10 - (hasScroll ? 20 : 0);

    std::vector<TLine> currentLine;
    std::wstring buffer;
    SDL_Color currentColor = { 0, 0, 0, 255 };
    int currentWidth = 0;

    int cursorX = 5;
    int cursorY = m_lineheight / 2;

    size_t i = 0;
    while (i < m_wtext.size())
    {
        // # 颜色处理
        if (m_wtext[i] == L'#' && i + 1 < m_wtext.size())
        {
            if (!buffer.empty()) {
                currentLine.push_back({ currentColor, buffer });
                buffer.clear();
            }

            if (i + 7 <= m_wtext.size()) {
                std::wstring hex = m_wtext.substr(i + 1, 6);
                if (std::regex_match(hex, std::wregex(L"[0-9a-fA-F]{6}"))) {
                    Color::HexToColor(hex, currentColor);
                    i += 7;
                    continue;
                }
            }

            static std::unordered_map<wchar_t, SDL_Color> m_colors = {
                {L'R', {255,0,0,255}}, {L'G', {0,255,0,255}}, {L'B', {0,0,255,255}},
                {L'C', {0,0,0,255}},   {L'P', {255, 20, 147,255}}, {L'O', {255, 165, 0,255}},
                {L'Y', {255, 255, 0,255}}, {L'F', {255, 0, 255,255}}, {L'A', {0, 255, 255,255}},
                {L'S', {244, 164, 96,255}}, {L'W', {255,255,255,255}}, {L'K', {0,0,0,255}},
                {L'D', {128, 128, 128,255}}
            };

            if (m_colors.find(m_wtext[i + 1]) != m_colors.end()) {
                currentColor = m_colors[m_wtext[i + 1]];
                i += 2;
                continue;
            }

            buffer += m_wtext[i];
            ++i;
            continue;
        }

        // \n 换行处理
        if (m_wtext[i] == L'\n') {
            if (!buffer.empty()) {
                currentLine.push_back({ currentColor, buffer });
                buffer.clear();
            }
            m_items.push_back(currentLine);
            currentLine.clear();
            currentWidth = 0;
            cursorX = 5;
            cursorY += m_lineheight + m_linespace;
            ++i;
            continue;
        }

        // [LINK tag=... value=...]...[/LINK] 解析
        if (m_wtext.compare(i, 5, L"[LINK") == 0) {
            size_t linkStart = i;
            size_t endBracket = m_wtext.find(L']', i);
            if (endBracket == std::wstring::npos) {
                buffer += m_wtext[i++];
                continue;
            }

            std::wstring header = m_wtext.substr(i + 1, endBracket - i - 1);  // LINK tag=.. value=..
            std::wsmatch match;
            std::wregex linkHeaderRe(LR"(LINK\s+tag=(\d+)\s+value=([^\]]+))");
            if (!std::regex_match(header, match, linkHeaderRe)) {
                buffer += m_wtext[i++];
                continue;
            }

            std::wstring tag = match[1].str();
            std::wstring value = match[2].str();

            size_t contentStart = endBracket + 1;
            size_t linkEnd = m_wtext.find(L"[/LINK]", contentStart);
            if (linkEnd == std::wstring::npos) {
                buffer += m_wtext[i++];
                continue;
            }

            std::wstring linkText = m_wtext.substr(contentStart, linkEnd - contentStart);
            auto [linkWidth, _] = sRender->GetTextSize(linkText.c_str(), m_fontSize);

            if (!buffer.empty()) {
                currentLine.push_back({ currentColor, buffer });
                buffer.clear();
            }

            // 若链接超过当前行，换行
            if (currentWidth + linkWidth > lineWidth) {
                m_items.push_back(currentLine);
                currentLine.clear();
                currentWidth = 0;
                cursorX = 5;
                cursorY += m_lineheight + m_linespace;
            }

            SDL_Rect rect = { cursorX, cursorY, linkWidth, m_lineheight };
            //m_links.push_back({{0,0,255,255}, linkText,true,  tag, value, rect });

            currentLine.push_back({ Color::GREEN, linkText,true,  tag, value, rect });

            currentWidth += linkWidth;
            cursorX += linkWidth;

            i = linkEnd + 7;
            continue;
        }

        // 普通字符
        wchar_t ch = m_wtext[i];
        buffer += ch;
        currentWidth += m_textsize[i];
        cursorX += m_textsize[i];

        // 自动换行
        if (currentWidth > lineWidth) {
            buffer.pop_back();
            cursorX -= m_textsize[i];

            if (!buffer.empty()) {
                currentLine.push_back({ currentColor, buffer });
                buffer.clear();
            }

            m_items.push_back(currentLine);
            currentLine.clear();
            currentWidth = 0;
            cursorX = 5;
            cursorY += m_lineheight + m_linespace;

            // 不跳过字符
            continue;
        }

        ++i;
    }

    if (!buffer.empty())
        currentLine.push_back({ currentColor, buffer });
    if (!currentLine.empty())
        m_items.push_back(currentLine);

    SetContentHeight((int)m_items.size());
}



void UITextField::InnerScrollVisiableChanged(bool _visiable)
{
    ResolveText();
}