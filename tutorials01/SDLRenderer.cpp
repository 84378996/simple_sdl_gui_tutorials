#include "SDLRenderer.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_syswm.h>
#include <vector>
#include <array>

int _DEFAULT_FONT_SIZE = 14;

SDLRenderer::SDLRenderer()
    : m_render(nullptr)
    , m_wnd(nullptr)
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
}

SDLRenderer::~SDLRenderer()
{
    if (m_render)
    {
        SDL_DestroyRenderer(m_render);
        m_render = nullptr;
    }

    if (m_wnd)
    {
        SDL_DestroyWindow(m_wnd);
        m_wnd = nullptr;
    }

    for (auto it : m_fonts)
    {
        if (it.second)
            TTF_CloseFont(it.second);
    }
    m_fonts.clear();
}

bool SDLRenderer::InitRender()
{
    m_wnd = SDL_CreateWindow("Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (m_wnd == nullptr) return false;
    m_render = SDL_CreateRenderer(m_wnd, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(m_render, SDL_BLENDMODE_BLEND);
    if (m_render == nullptr) return false;
    SDL_GetWindowSize(m_wnd, &m_width, &m_height);
    return true;
}

bool SDLRenderer::SetFontFile(const std::string& ff)
{
    m_fontfile = ff;
    return true;
}

void SDLRenderer::DrawLine(const SDL_Point& p1, const SDL_Point& p2, const SDL_Color& c)
{
    SetColorHelper _help(c);
    DrawLine(p1, p2);
}

void SDLRenderer::DrawLine(int x1, int y1, int x2, int y2, const SDL_Color& c)
{
    DrawLine({ x1,y1 }, { x2,y2 }, c);
}


void SDLRenderer::DrawLine(const SDL_Point& p1, const SDL_Point& p2)
{
    SDL_RenderDrawLine(m_render, p1.x, p1.y, p2.x, p2.y);
}


void SDLRenderer::DrawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(m_render, x1, y1, x2, y2);
}

void SDLRenderer::DrawLines(const SDL_Point* p, int len, const SDL_Color& c)
{
    SetColorHelper _help(c);
    SDL_RenderDrawLines(m_render, p, len);
}

void SDLRenderer::DrawLines(const SDL_Point* p, int len)
{
    SDL_RenderDrawLines(m_render, p, len);
}

void SDLRenderer::DrawLines(int* x, int* y, int len)
{
    std::vector<SDL_Point> pts;
    for (int i = 0; i < len; i++)
        pts.push_back({ x[i],y[i] });
    DrawLines(pts.data(), (int)pts.size());
}

void SDLRenderer::DrawLines(int* x, int* y, int len, const SDL_Color& c)
{
    SetColorHelper _help(c);
    DrawLines(x, y, len);
}

void SDLRenderer::DrawRect(const SDL_Rect& rc, const SDL_Color& c)
{
    SetColorHelper _help(c);
    DrawRect(rc);
}

void SDLRenderer::DrawRect(const SDL_Rect& rc)
{
    SDL_RenderDrawRect(m_render, &rc);
}

void SDLRenderer::FillRect(const SDL_Rect& rc, const SDL_Color& c)
{
    SetColorHelper _help(c);
    FillRect(rc);
}

void SDLRenderer::FillRect(const SDL_Rect& rc)
{
    SDL_RenderFillRect(m_render, &rc);
}

void SDLRenderer::DrawCircle(const SDL_Rect& rc, const SDL_Color& c)
{
    if (rc.h != rc.w) return;
    SetColorHelper _help(c);
    DrawCircle(rc);
}

void SDLRenderer::DrawCircle(const SDL_Rect& rc)
{
    /*if (rc.h != rc.w) return;
    Sint16 x = (Sint16)(rc.x + rc.w / 2);
    Sint16 rx = (Sint16)(rc.w / 2);
    Sint16 y = (Sint16)(rc.y + rc.h / 2);
    Sint16 ry = (Sint16)(rc.h / 2);
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(m_render, &r, &g, &b, &a);
    if (rx == ry)
        circleRGBA(m_render, x, y, rx, r, g, b, a);
    else
        ellipseRGBA(m_render, x, y, rx, ry, r, g, b, a);*/
}

void SDLRenderer::FillCircle(const SDL_Rect& rc, const SDL_Color& c)
{
    if (rc.h != rc.w) return;
    SetColorHelper _help(c);
    FillCircle(rc);
}

void SDLRenderer::FillCircle(const SDL_Rect& rc)
{
    /*if (rc.h != rc.w) return;
    Sint16 x = (Sint16)(rc.x + rc.w / 2);
    Sint16 rx = (Sint16)(rc.w / 2);
    Sint16 y = (Sint16)(rc.y + rc.h / 2);
    Sint16 ry = (Sint16)(rc.h / 2);
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(m_render, &r, &g, &b, &a);
    if (rx == ry)
        filledCircleRGBA(m_render, x, y, rx, r, g, b, a);
    else
        filledEllipseRGBA(m_render, x, y, rx, ry, r, g, b, a);*/
}

void SDLRenderer::DrawImage(SDL_Texture* _tex)
{
    int w = 0;
    int h = 0;
    SDL_QueryTexture(_tex, nullptr, nullptr, &w, &h);
    SDL_Rect rc{ 0, 0, w, h };
    DrawImage(_tex, rc, rc);
}

void SDLRenderer::DrawImage(SDL_Texture* _tex, const SDL_Point& pt)
{
    int w = 0;
    int h = 0;
    SDL_QueryTexture(_tex, nullptr, nullptr, &w, &h);
    SDL_Rect src{ 0,0,w,h };
    SDL_Rect dst{ pt.x, pt.y, w, h };
    DrawImage(_tex, src, dst);
}

void SDLRenderer::DrawImage(SDL_Texture* _tex, int x, int y)
{
    SDL_Point pt = { x,y };
    DrawImage(_tex, pt);
}

void SDLRenderer::DrawImage(SDL_Texture* _tex, const SDL_Rect& src)
{
    if (_tex == nullptr) return;
    int w = 0;
    int h = 0;
    SDL_QueryTexture(_tex, nullptr, nullptr, &w, &h);
    SDL_Rect src1{ 0,0,w,h };
    SDL_Rect dst{ src.x + (src.w - w) / 2  , src.y + (src.h - h) / 2, w, h };
    DrawImage(_tex, src1, dst);
}

void SDLRenderer::DrawImage(SDL_Texture* _tex, const SDL_Rect& src, const SDL_Rect& dst)
{
    /*SDL_SetTextureBlendMode(_tex, SDL_BLENDMODE_BLEND);
    if (SDL_RenderCopyEx(m_render, _tex, &src, &dst, 0, 0, SDL_RendererFlip::SDL_FLIP_NONE) != 0)
    {
        SDL_assert(false);
    }*/
    DrawImage(_tex, &src, &dst);
}

void SDLRenderer::DrawImage(SDL_Texture* _tex, const SDL_Rect* src, const SDL_Rect* dst)
{
    SDL_SetTextureBlendMode(_tex, SDL_BLENDMODE_BLEND);
    if (SDL_RenderCopy(m_render, _tex, src, dst) != 0)
    {
        SDL_assert(false);
    }
}


void SDLRenderer::Draw9Image(SDL_Texture* _tex, const SDL_Rect& destRect, const float* rate)
{
    auto [w, h] = GetTextureSize(_tex);
    SDL_Rect srcRect{ 0, 0, w, h };

    int borderLeft = static_cast<int>(w * 0.3);
    int borderRight = static_cast<int>(w * 0.3);
    int borderTop = static_cast<int>(h * 0.3);
    int borderBottom = static_cast<int>(h * 0.3);

    // Calculate the border sizes in the source image based on the given rates
    if (rate != nullptr)
    {
        borderLeft = static_cast<int>(w * rate[0]);
        borderRight = static_cast<int>(w * rate[1]);
        borderTop = static_cast<int>(h * rate[2]);
        borderBottom = static_cast<int>(h * rate[3]);
        // Ensure valid input (sum of horizontal and vertical rates should be less than 1)
        if (rate[0] + rate[1] >= 1.0f || rate[2] + rate[3] >= 1.0f) {
            SDL_Log("Invalid rate values. Sum of horizontal or vertical rates must be less than 1.");
            return;
        }
    }

    if (borderLeft > destRect.w / 2)
        borderLeft = destRect.w / 2;
    if (borderRight > destRect.w / 2)
        borderRight = destRect.w / 2;

    if (borderTop > destRect.h / 2)
        borderTop = destRect.h / 2;
    if (borderBottom > destRect.h / 2)
        borderBottom = destRect.h / 2;


    // Define the 9 parts of the source texture based on the calculated border sizes
    SDL_Rect src[9], dest[9];

    // Top-left corner
    src[0] = { srcRect.x, srcRect.y, borderLeft, borderTop };
    dest[0] = { destRect.x, destRect.y, borderLeft, borderTop };

    // Top-right corner
    src[2] = { srcRect.x + srcRect.w - borderRight, srcRect.y, borderRight, borderTop };
    dest[2] = { destRect.x + destRect.w - borderRight, destRect.y, borderRight, borderTop };

    // Bottom-left corner
    src[6] = { srcRect.x, srcRect.y + srcRect.h - borderBottom, borderLeft, borderBottom };
    dest[6] = { destRect.x, destRect.y + destRect.h - borderBottom, borderLeft, borderBottom };

    // Bottom-right corner
    src[8] = { srcRect.x + srcRect.w - borderRight, srcRect.y + srcRect.h - borderBottom, borderRight, borderBottom };
    dest[8] = { destRect.x + destRect.w - borderRight, destRect.y + destRect.h - borderBottom, borderRight, borderBottom };

    // Draw the four corners
    for (int i = 0; i < 9; i += 2) {
        SDL_RenderCopy(m_render, _tex, &src[i], &dest[i]);
    }

    // Top and bottom borders (tiled horizontally)
    SDL_Rect topSrc = { srcRect.x + borderLeft, srcRect.y, srcRect.w - borderLeft - borderRight, borderTop };
    SDL_Rect bottomSrc = { srcRect.x + borderLeft, srcRect.y + srcRect.h - borderBottom, srcRect.w - borderLeft - borderRight, borderBottom };
    for (int x = destRect.x + borderLeft; x < destRect.x + destRect.w - borderRight; x += topSrc.w) {
        int width = (x + topSrc.w > destRect.x + destRect.w - borderRight) ? (destRect.x + destRect.w - borderRight - x) : topSrc.w;

        // Top border
        SDL_Rect topDest = { x, destRect.y, width, borderTop };
        SDL_Rect topSrcClip = { topSrc.x, topSrc.y, width, topSrc.h };
        SDL_RenderCopy(m_render, _tex, &topSrcClip, &topDest);

        // Bottom border
        SDL_Rect bottomDest = { x, destRect.y + destRect.h - borderBottom, width, borderBottom };
        SDL_Rect bottomSrcClip = { bottomSrc.x, bottomSrc.y, width, bottomSrc.h };
        SDL_RenderCopy(m_render, _tex, &bottomSrcClip, &bottomDest);
    }

    // Left and right borders (tiled vertically)
    SDL_Rect leftSrc = { srcRect.x, srcRect.y + borderTop, borderLeft, srcRect.h - borderTop - borderBottom };
    SDL_Rect rightSrc = { srcRect.x + srcRect.w - borderRight, srcRect.y + borderTop, borderRight, srcRect.h - borderTop - borderBottom };
    for (int y = destRect.y + borderTop; y < destRect.y + destRect.h - borderBottom; y += leftSrc.h) {
        int height = (y + leftSrc.h > destRect.y + destRect.h - borderBottom) ? (destRect.y + destRect.h - borderBottom - y) : leftSrc.h;

        // Left border
        SDL_Rect leftDest = { destRect.x, y, borderLeft, height };
        SDL_Rect leftSrcClip = { leftSrc.x, leftSrc.y, leftSrc.w, height };
        SDL_RenderCopy(m_render, _tex, &leftSrcClip, &leftDest);

        // Right border
        SDL_Rect rightDest = { destRect.x + destRect.w - borderRight, y, borderRight, height };
        SDL_Rect rightSrcClip = { rightSrc.x, rightSrc.y, rightSrc.w, height };
        SDL_RenderCopy(m_render, _tex, &rightSrcClip, &rightDest);
    }

    // Center (stretched)
    SDL_Rect centerSrc = { srcRect.x + borderLeft, srcRect.y + borderTop, srcRect.w - borderLeft - borderRight, srcRect.h - borderTop - borderBottom };
    SDL_Rect centerDest = { destRect.x + borderLeft, destRect.y + borderTop, destRect.w - borderLeft - borderRight, destRect.h - borderTop - borderBottom };
    SDL_RenderCopy(m_render, _tex, &centerSrc, &centerDest);
}

void SDLRenderer::DrawToImage(SDL_Texture* src, SDL_Texture** dst, int dstwidth, int dstheight)
{
    if (!src || !dst || dstwidth <= 0 || dstheight <= 0) return;

    SDL_Renderer* renderer = sRender->GetRender();
    if (!renderer) {
        SDL_Log("Failed to get renderer from src texture: %s", SDL_GetError());
        return;
    }

    // 创建目标纹理（可渲染）
    SDL_Texture* newDst = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        dstwidth,
        dstheight);
    if (!newDst) {
        SDL_Log("Failed to create target texture: %s", SDL_GetError());
        return;
    }

    // 保存当前渲染目标
    SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);

    // 设置渲染目标为新建的目标纹理
    SDL_SetRenderTarget(renderer, newDst);

    // 可选：清空为透明背景
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // 渲染源纹理到目标纹理（会自动缩放）
    SDL_Rect dstRect = { 0, 0, dstwidth, dstheight };
    //SDL_RenderCopy(renderer, src, NULL, &dstRect);
    sRender->Draw9Image(src, dstRect);

    // 恢复渲染目标
    SDL_SetRenderTarget(renderer, oldTarget);

    // 赋值给输出参数
    *dst = newDst;
}

void SDLRenderer::DrawString(const char* text, const SDL_Point& pt)
{
    SDL_Color c{ 0, 0, 0, 255 };
    DrawString(text, pt, c);
}

void SDLRenderer::DrawString(const char* text, const SDL_Rect& rc)
{
    DrawString(text, _DEFAULT_FONT_SIZE, rc);
}

void SDLRenderer::DrawString(const char* text, int fontsize, const SDL_Rect& rc)
{
    TTF_Font* font = GetFont(fontsize);
    int w, h;
    TTF_SizeUTF8(font, text, &w, &h);
    int x, y;
    x = rc.x + 2;
    y = rc.y + (rc.h - h) / 2;
    DrawString(text, font, SDL_Point{ x,y });
}

void SDLRenderer::DrawString(const char* text, int fontsize, const SDL_Rect& rc, const SDL_Color& c)
{
    TTF_Font* font = GetFont(fontsize);
    int w, h;
    TTF_SizeUTF8(font, text, &w, &h);
    int x, y;
    if (rc.w - 2 > w)
        x = rc.x + 2;
    else
        x = rc.x - (w - rc.w) - 4;
    y = rc.y + (rc.h - h) / 2;
    DrawString(text, font, SDL_Point{ x,y }, c);
}

void SDLRenderer::DrawString2(const char* text, int fontsize, const SDL_Rect& rc, const SDL_Color& c)
{
    TTF_Font* font = GetFont(fontsize);
    int w, h;
    TTF_SizeUTF8(font, text, &w, &h);
    int x, y;
    x = rc.x + (rc.w - w) / 2;
    y = rc.y + (rc.h - h) / 2;
    DrawString(text, font, SDL_Rect{ x,y,w,h }, c);
}

void SDLRenderer::DrawString(const char* text, int fontsize, const SDL_Point& pt)
{
    SDL_Color c{ 0, 0, 0, 255 };
    DrawString(text, fontsize, pt, c);
}

void SDLRenderer::DrawString(const char* text, TTF_Font* font, const SDL_Point& pt)
{
    SDL_Color c{ 0, 0, 0, 255 };
    DrawString(text, font, pt, c);
}

void SDLRenderer::DrawString(const char* text, const SDL_Point& pt, const SDL_Color& c)
{
    DrawString(text, _DEFAULT_FONT_SIZE, pt, c);
}

void SDLRenderer::DrawString(const char* text, int fontsize, const SDL_Point& pt, const SDL_Color& c)
{
    TTF_Font* font = GetFont(fontsize);
    DrawString(text, font, pt, c);
}

void SDLRenderer::DrawString(const char* text, TTF_Font* font, const SDL_Point& dst, const SDL_Color& c)
{
    if (!strlen(text)) return;
    SDL_Surface* sur = TTF_RenderUTF8_Blended(font, text, c);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_render, sur);
    SDL_FreeSurface(sur);
    DrawImage(tex, dst);
    SDL_DestroyTexture(tex);
}

void SDLRenderer::DrawString(const char* text, TTF_Font* font, const SDL_Rect& dst, const SDL_Color& c)
{
    if (!strlen(text)) return;
    SDL_Surface* sur = TTF_RenderUTF8_Blended(font, text, c);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_render, sur);
    SDL_FreeSurface(sur);
    auto [w, h] = sRender->GetTextureSize(tex);
    DrawImage(tex, dst.x + (dst.w - w) / 2, dst.y + (dst.h - h) / 2);
    SDL_DestroyTexture(tex);
}

void SDLRenderer::DrawMulitLineString(const std::vector<std::string>& strs, int fontsize, const SDL_Rect& rect, const SDL_Color& c, int linespace)
{
    TTF_Font* font = GetFont(fontsize);
    if (!font || strs.empty())
        return;

    int w = 0, h = 0, maxWidth = 0, totalHeight = 0;

    // 计算所有行的最大宽度与总高度
    std::vector<SDL_Surface*> surfaces;
    for (const auto& s : strs)
    {
        if (TTF_SizeUTF8(font, s.c_str(), &w, &h) == 0)
        {
            totalHeight += h;
            if (!surfaces.emplace_back(TTF_RenderUTF8_Blended(font, s.c_str(), c))) {
                SDL_Log("TTF_RenderUTF8_Blended failed: %s", TTF_GetError());
                continue;
            }
            if (maxWidth < w)
                maxWidth = w;
        }
        else
        {
            SDL_Log("TTF_SizeUTF8 failed: %s", TTF_GetError());
        }
    }

    // 增加行间距
    totalHeight += static_cast<int>((strs.size() - 1) * linespace);

    // 起始绘制位置（垂直居中）
    int y = rect.y + (rect.h - totalHeight) / 2;

    for (auto surface : surfaces)
    {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_render, surface);
        if (!texture) {
            SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
            SDL_FreeSurface(surface);
            continue;
        }

        SDL_Rect dst;
        dst.w = surface->w;
        dst.h = surface->h;
        dst.x = rect.x + (rect.w - dst.w) / 2; // 水平居中
        dst.y = y;

        SDL_RenderCopy(m_render, texture, nullptr, &dst);
        y += dst.h + linespace;

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

void SDLRenderer::DrawVerticalText(TTF_Font* font, const std::wstring& text, int x, int y, SDL_Color color, int lineSpacing)
{
    int offsetY = 0;

    for (uint16_t c : text) {
        // 渲染单个字符为 surface
        std::wstring s(1, c);
        SDL_Surface* surface = TTF_RenderUNICODE_Blended(font, (Uint16*)s.c_str(), color);
        if (!surface) continue;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_render, surface);
        SDL_Rect dst = { x, y + offsetY, surface->w, surface->h };

        SDL_RenderCopy(m_render, texture, nullptr, &dst);

        offsetY += surface->h + lineSpacing;

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}


void SDLRenderer::DrawString(const wchar_t* text, const SDL_Point& pt)
{
    SDL_Color c{ 0, 0, 0, 255 };
    DrawString(text, pt, c);
}

void SDLRenderer::DrawString(const wchar_t* text, const SDL_Rect& rc)
{
    DrawString(text, _DEFAULT_FONT_SIZE, rc);
}

void SDLRenderer::DrawString(const wchar_t* text, int fontsize, const SDL_Rect& rc)
{
    TTF_Font* font = GetFont(fontsize);
    int w, h;
    TTF_SizeUNICODE(font, (Uint16*)text, &w, &h);
    int x, y;
    x = rc.x + 2;
    y = rc.y + (rc.h - h) / 2;
    DrawString(text, font, SDL_Point{ x,y });
}

void SDLRenderer::DrawString(const wchar_t* text, int fontsize, const SDL_Rect& rc, const SDL_Color& c)
{
    TTF_Font* font = GetFont(fontsize);
    int w, h;
    TTF_SizeUNICODE(font, (Uint16*)text, &w, &h);
    int x, y;
    if (rc.w - 2 > w)
        x = rc.x + 2;
    else
        x = rc.x - (w - rc.w) - 4;
    y = rc.y + (rc.h - h) / 2;
    DrawString(text, font, SDL_Point{ rc.x,y }, c);
}

void SDLRenderer::DrawString(const wchar_t* text, int fontsize, const SDL_Point& pt)
{
    SDL_Color c{ 0, 0, 0, 255 };
    DrawString(text, fontsize, pt, c);
}

void SDLRenderer::DrawString(const wchar_t* text, TTF_Font* font, const SDL_Point& pt)
{
    SDL_Color c{ 0, 0, 0, 255 };
    DrawString(text, font, pt, c);
}

void SDLRenderer::DrawString(const wchar_t* text, const SDL_Point& pt, const SDL_Color& c)
{
    DrawString(text, _DEFAULT_FONT_SIZE, pt, c);
}

void SDLRenderer::DrawString(const wchar_t* text, int fontsize, const SDL_Point& pt, const SDL_Color& c)
{
    TTF_Font* font = GetFont(fontsize);
    DrawString(text, font, pt, c);
}

void SDLRenderer::DrawString(const wchar_t* text, TTF_Font* font, const SDL_Point& dst, const SDL_Color& c)
{
    if (!wcslen(text)) return;
    SDL_Surface* sur = TTF_RenderUNICODE_Blended(font, (Uint16*)text, c);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_render, sur);
    SDL_FreeSurface(sur);
    DrawImage(tex, dst);
    SDL_DestroyTexture(tex);
}

void SDLRenderer::DrawString(const wchar_t* text, TTF_Font* font, const SDL_Rect& dst, const SDL_Color& c)
{
    if (!wcslen(text)) return;
    SDL_Surface* sur = TTF_RenderUNICODE_Blended(font, (Uint16*)text, c);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_render, sur);
    SDL_FreeSurface(sur);
    auto [w, h] = sRender->GetTextureSize(tex);
    DrawImage(tex, dst.x + (dst.w - w) / 2, dst.y + (dst.h - h) / 2);
    SDL_DestroyTexture(tex);
}

SDL_Texture* SDLRenderer::CreateSolidColorTexture(int width, int height, SDL_Color color) {
    if (!m_render || width <= 0 || height <= 0) return nullptr;

    // 创建支持写入像素的 streaming texture
    SDL_Texture* texture = SDL_CreateTexture(m_render,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        width, height);
    if (!texture) return nullptr;

    // 锁定纹理进行像素填充
    void* pixels;
    int pitch;
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
        SDL_DestroyTexture(texture);
        return nullptr;
    }

    Uint32 pixelValue = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32),
        color.r, color.g, color.b, color.a);

    // 填充所有像素为相同颜色
    for (int y = 0; y < height; ++y) {
        Uint32* row = (Uint32*)((Uint8*)pixels + y * pitch);
        for (int x = 0; x < width; ++x) {
            row[x] = pixelValue;
        }
    }

    SDL_UnlockTexture(texture);
    return texture;
}


SDL_Window* SDLRenderer::GetWindow()
{
    return m_wnd;
}

void* SDLRenderer::GetWindowHandle()
{
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(m_wnd, &wmInfo);
    return wmInfo.info.win.window;
}

SDL_Color SDLRenderer::GetRenderColor()
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(m_render, &r, &g, &b, &a);
    return { r,g,b,a };
}

void SDLRenderer::SetRenderColor(const SDL_Color* c)
{
    /*if (c->a < 255)
        SDL_SetRenderDrawBlendMode(m_render, SDL_BLENDMODE_BLEND);
    else
        SDL_SetRenderDrawBlendMode(m_render, SDL_BLENDMODE_NONE);*/
    SDL_SetRenderDrawColor(m_render, c->r, c->g, c->b, c->a);
}


SDL_Renderer* SDLRenderer::GetRender()
{
    return m_render;
}

TTF_Font* SDLRenderer::GetFont(int _fontsize)
{
    if (m_fonts.find(_fontsize) != m_fonts.end())
        return m_fonts[_fontsize];
    else
    {
        TTF_Font* ft = TTF_OpenFont(m_fontfile.c_str(), _fontsize);
        if (ft)
        {
            m_fonts.emplace(_fontsize, ft);
            return ft;
        }
        return nullptr;
    }
}

SDL_Rect SDLRenderer::GetClientRect()
{
    return { 0,0,m_width,m_height };
}

std::tuple<int, int> SDLRenderer::GetClientSize()
{
    return { m_width,m_height };
}

void SDLRenderer::SetClientSize(int width, int height)
{
    if (width <= 0 || height <= 0) return;

    if (m_wnd)
    {
        SDL_SetWindowSize(m_wnd, width, height);
        m_width = width;
        m_height = height;
    }
}

void SDLRenderer::GetClipRect(SDL_Rect* rc)
{
    SDL_RenderGetClipRect(m_render, rc);
}

void SDLRenderer::SetClipRect(const SDL_Rect* rc)
{
    SDL_RenderSetClipRect(m_render, rc);
}

SDL_Texture* SDLRenderer::LoadTextureFromFile(const char* file)
{
    auto sur = IMG_Load(file);
    auto tex = SDL_CreateTextureFromSurface(m_render, sur);
    SDL_FreeSurface(sur);
    return tex;
}

std::tuple<int, int> SDLRenderer::GetTextureSize(SDL_Texture* _tex)
{
    int h, w;
    SDL_QueryTexture(_tex, nullptr, nullptr, &w, &h);
    return { w,h };
}

std::tuple<int, int> SDLRenderer::GetTextSize(const char* _text, int _fontsize)
{
    TTF_Font* font = GetFont(_fontsize);
    int w, h;
    TTF_SizeUTF8(font, _text, &w, &h);
    return { w,h };
}

int SDLRenderer::GetTextHeight(int _fontsize)
{
    auto font = GetFont(_fontsize);
    return TTF_FontHeight(font);
}

std::tuple<int, int> SDLRenderer::GetTextSize(const wchar_t* _text, int _fontsize)
{
    TTF_Font* font = GetFont(_fontsize);
    int w, h;
    TTF_SizeUNICODE(font, (Uint16*)_text, &w, &h);
    return { w,h };
}

std::vector<int> SDLRenderer::GetTextSizes(const std::string& utf8string, int _fontsize)
{
    std::vector<int> ret;
    TTF_Font* font = GetFont(_fontsize);
    char szc[2] = { 0 };
    int w, h;
    for (size_t i = 0; i < utf8string.size(); i++)
    {
        szc[0] = utf8string[i];
        TTF_SizeUTF8(font, szc, &w, &h);
        ret.push_back(w);
        ret.push_back(h);
    }
    return ret;
}

std::vector<int> SDLRenderer::GetTextSizes(const std::wstring& utf8string, int _fontsize)
{
    std::vector<int> ret;
    TTF_Font* font = GetFont(_fontsize);
    wchar_t szc[2] = { 0 };
    int w, h;
    for (size_t i = 0; i < utf8string.size(); i++)
    {
        szc[0] = utf8string[i];
        TTF_SizeUNICODE(font, (Uint16*)szc, &w, &h);
        ret.push_back(w);
        ret.push_back(h);
    }
    return ret;
}

std::vector<int> SDLRenderer::GetTextWidths(const std::wstring& utf8string, int _fontsize)
{
    std::vector<int> ret;
    TTF_Font* font = GetFont(_fontsize);
    wchar_t szc[2] = { 0 };
    int w;
    for (size_t i = 0; i < utf8string.size(); i++)
    {
        szc[0] = utf8string[i];
        TTF_SizeUNICODE(font, (Uint16*)szc, &w, 0);
        ret.push_back(w);
    }
    return ret;
}

void SDLRenderer::setColor(const SDL_Color& c)
{
    if (c.a < 255)
        SDL_SetRenderDrawBlendMode(m_render, SDL_BLENDMODE_BLEND);
    else
        SDL_SetRenderDrawBlendMode(m_render, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(m_render, c.r, c.g, c.b, c.a);
}

SDL_Color SDLRenderer::getColor() const
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(m_render, &r, &g, &b, &a);
    return SDL_Color{ r, g, b, a };
}

void SDLRenderer::Present()
{
    SDL_RenderPresent(m_render);
}

void SDLRenderer::Clear()
{
    SDL_SetRenderDrawBlendMode(m_render, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(m_render, 0, 0, 0, 255);
    SDL_RenderClear(m_render);
}