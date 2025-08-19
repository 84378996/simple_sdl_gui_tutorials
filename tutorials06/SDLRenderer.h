#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern int _DEFAULT_FONT_SIZE;

class SDLRenderer
{
public:
	SDLRenderer();
	~SDLRenderer();

	bool InitRender();
	bool SetFontFile(const std::string& ff);
	void DrawLine(const SDL_Point& p1, const SDL_Point& p2, const SDL_Color& c);
	void DrawLine(const SDL_Point& p1, const SDL_Point& p2);
	void DrawLine(int x1,int y1,int x2, int y2, const SDL_Color& c);
	void DrawLine(int x1, int y1, int x2, int y2);
	void DrawLines(const SDL_Point* p, int len, const SDL_Color& c);
	void DrawLines(const SDL_Point* p, int len);
	void DrawLines(int *x, int *y, int len);
	void DrawLines(int *x, int *y, int len, const SDL_Color& c);
	void DrawRect(const SDL_Rect& rc, const SDL_Color& c);
	void DrawRect(const SDL_Rect& rc);
	void FillRect(const SDL_Rect& rc, const SDL_Color& c);
	void FillRect(const SDL_Rect& rc);
	void DrawCircle(const SDL_Rect& rc, const SDL_Color& c);
	void DrawCircle(const SDL_Rect& rc);
	void FillCircle(const SDL_Rect& rc, const SDL_Color& c);
	void FillCircle(const SDL_Rect& rc);
	void DrawImage(SDL_Texture* _tex);
	void DrawImage(SDL_Texture* _tex, const SDL_Point& pt);
	void DrawImage(SDL_Texture* _tex, int x, int  y );
	void DrawImage(SDL_Texture* _tex, const SDL_Rect& src);
	void DrawImage(SDL_Texture* _tex, const SDL_Rect& src, const SDL_Rect& dst);
	void DrawImage(SDL_Texture* _tex, const SDL_Rect* src, const SDL_Rect* dst);
	void Draw9Image(SDL_Texture* _tex, const SDL_Rect& dst, const float* r = nullptr);
	void DrawToImage(SDL_Texture* src, SDL_Texture** dst, int width, int height);

	void DrawString(const char* text, const SDL_Point& pt);
	void DrawString(const char* text, const SDL_Rect& rc);
	void DrawString(const char* text, int fontsize, const SDL_Rect& rc);
	void DrawString(const char* text, int fontsize, const SDL_Rect& rc, const SDL_Color& c);
	void DrawString2(const char* text, int fontsize, const SDL_Rect& rc, const SDL_Color& c);
	void DrawString(const char* text, int fontsize, const SDL_Point& pt);
	void DrawString(const char* text, TTF_Font* font, const SDL_Point& dst);
	void DrawString(const char* text, const SDL_Point& pt, const SDL_Color& c);
	void DrawString(const char* text, int fontsize, const SDL_Point& pt, const SDL_Color& c);
	void DrawString(const char* text, TTF_Font* font, const SDL_Point& dst, const SDL_Color& c);
	void DrawString(const char* text, TTF_Font* font, const SDL_Rect& dst, const SDL_Color& c);

	void DrawString(const wchar_t* text, const SDL_Point& pt);
	void DrawString(const wchar_t* text, const SDL_Rect& rc);
	void DrawString(const wchar_t* text, int fontsize, const SDL_Rect& rc);
	void DrawString(const wchar_t* text, int fontsize, const SDL_Rect& rc, const SDL_Color& c);
	void DrawString(const wchar_t* text, int fontsize, const SDL_Point& pt);
	void DrawString(const wchar_t* text, TTF_Font* font, const SDL_Point& dst);
	void DrawString(const wchar_t* text, const SDL_Point& pt, const SDL_Color& c);
	void DrawString(const wchar_t* text, int fontsize, const SDL_Point& pt, const SDL_Color& c);
	void DrawString(const wchar_t* text, TTF_Font* font, const SDL_Point& dst, const SDL_Color& c);
	void DrawString(const wchar_t* text, TTF_Font* font, const SDL_Rect& dst, const SDL_Color& c);

	void DrawMulitLineString(const std::vector<std::string>& strs, int fontsize, const SDL_Rect& rect, const SDL_Color& c, int linespace);
	void DrawVerticalText(TTF_Font* font, const std::wstring& text, int x, int y, SDL_Color color, int lineSpacing = 0);
	SDL_Texture* CreateSolidColorTexture(int width, int height, SDL_Color color);

	void Present();
	void Clear();

	SDL_Window* GetWindow();
	void* GetWindowHandle();
	SDL_Color GetRenderColor();
	void SetRenderColor(const SDL_Color *c);
	SDL_Renderer* GetRender();
	TTF_Font* GetFont(int _fontsize);
	SDL_Rect GetClientRect();
	std::tuple<int,int> GetClientSize();
	void SetClientSize(int width, int height);

	void GetClipRect(SDL_Rect *rc);
	void SetClipRect(const SDL_Rect* rc);

	SDL_Texture* LoadTextureFromFile(const char* file);
	std::tuple<int, int> GetTextureSize(SDL_Texture* _tex);
	std::tuple<int, int> GetTextSize(const char* _text, int _fontsize);
	int GetTextHeight(int _fontsize);
	std::tuple<int, int> GetTextSize(const wchar_t* _text, int _fontsize);
	std::vector<int> GetTextSizes(const std::string& utf8string, int _fontsize);
	std::vector<int> GetTextSizes(const std::wstring& utf8string, int _fontsize);
	std::vector<int> GetTextWidths(const std::wstring& utf8string, int _fontsize);

public:
	static SDLRenderer* Instance()
	{
		static SDLRenderer sd;
		return &sd;
	}

private:
	void setColor(const SDL_Color& c);
	SDL_Color getColor() const;

private:
	std::string m_fontfile;
	SDL_Window* m_wnd;
	SDL_Renderer* m_render;
	SDL_Color m_color{ 0,0,0,255 };
	int m_width{ 0 };
	int m_height{ 0 };
	std::unordered_map<int, TTF_Font*> m_fonts;
};

#define sRender SDLRenderer::Instance()

struct NewFrame
{
	NewFrame()
	{
		sRender->Clear();
	}
	~NewFrame()
	{
		sRender->Present();
	}
};

struct SetColorHelper
{
	SetColorHelper(SDL_Color c)
	{
		oc = sRender->GetRenderColor();
		sRender->SetRenderColor(&c);
	}

	~SetColorHelper()
	{
		sRender->SetRenderColor(&oc);
	}

private:
	SDL_Color oc;
};