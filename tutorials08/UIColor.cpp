#include "UIColor.h"
#include "SDLRenderer.h"

SDL_Color Color::RED = { 255,0,0,255 };
SDL_Color Color::GREEN = { 0,255,0,255 };
SDL_Color Color::BLUE = { 0,0,255,255 };
SDL_Color Color::BLACK = { 0,0,0,255 };
SDL_Color Color::PINK = { 255, 20, 147,255 };
SDL_Color Color::ORANGE = { 255, 165, 0,255 };
SDL_Color Color::YELLOW = { 255, 255, 0,255 };
SDL_Color Color::YANGRED = { 255, 0, 255,255 };
SDL_Color Color::PURPLE = { 128, 0, 128,255 };
SDL_Color Color::XYPURPLE = { 0x6b,0x23,0x8e,0xff };
SDL_Color Color::WHITE = { 255,255,255,255 };
SDL_Color Color::GRAY = { 102,102,102,255 };
SDL_Color Color::BUTTON = { 189,162,74,255 };

Color::Color()
	:
	r(0),
	g(0),
	b(0),
	a(255)
{

}

Color::Color(int color)
	:
	r((color >> 16) & 0xFF),
	b((color >> 8) & 0xFF),
	g((color >> 8) & 0xFF),
	a(255)
{

}

Color::Color(int ar, int ag, int ab, int aa)
	:
	r(ar),
	g(ag),
	b(ab),
	a(aa)
{

}

Color Color::operator+(const Color& color) const
{
	Color result(r + color.r, g + color.g, b + color.b, 255);

	result.r = (result.r > 255 ? 255 : (result.r < 0 ? 0 : result.r));
	result.g = (result.g > 255 ? 255 : (result.g < 0 ? 0 : result.g));
	result.b = (result.b > 255 ? 255 : (result.b < 0 ? 0 : result.b));

	return result;
}

Color Color::operator-(const Color& color) const
{
	Color result(r - color.r, g - color.g, b - color.b, 255);

	result.r = (result.r > 255 ? 255 : (result.r < 0 ? 0 : result.r));
	result.g = (result.g > 255 ? 255 : (result.g < 0 ? 0 : result.g));
	result.b = (result.b > 255 ? 255 : (result.b < 0 ? 0 : result.b));

	return result;
}

Color Color::operator*(float value) const
{
	Color result((int)(r * value), (int)(g * value), (int)(b * value), a);

	result.r = (result.r > 255 ? 255 : (result.r < 0 ? 0 : result.r));
	result.g = (result.g > 255 ? 255 : (result.g < 0 ? 0 : result.g));
	result.b = (result.b > 255 ? 255 : (result.b < 0 ? 0 : result.b));

	return result;
}

bool Color::operator==(const Color& color) const
{
	return r == color.r && g == color.g && b == color.b && a == color.a;
}

bool Color::operator!=(const Color& color) const
{
	return !(r == color.r && g == color.g && b == color.b && a == color.a);
}

Color::operator SDL_Color() const
{
	return SDL_Color{ (Uint8)r,(Uint8)g,(Uint8)b,(Uint8)a };
}

void Color::HexToColor(const std::wstring& hex, SDL_Color& c)
{
	c = SDL_Color{ 0,0,0,255 };
	if (hex.size() != 6) return;

	int r, g, b;
	try {
		r = std::stoi(hex.substr(0, 2), nullptr, 16);
		g = std::stoi(hex.substr(2, 2), nullptr, 16);
		b = std::stoi(hex.substr(4, 2), nullptr, 16);
	}
	catch (...) {
		return;
	}

	c = { static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), 255 };
}

SDL_Color Color::GetMoneyColor(uint64_t money)
{
	if (money < 100000)
		return SDL_Color{ 0x25,0xda,0x77 };
	else if (money >= 100000 && money < 1000000)
		return SDL_Color{ 0xfc,0x45,0xdc };
	else if (money >= 1000000 && money < 10000000)
		return SDL_Color{ 0xfb,0xd8,0x33 };
	else if (money >= 10000000 && money < 100000000)
		return SDL_Color{ 0x04,0xfd,0xf4 };
	else if (money >= 100000000 && money < 1000000000)
		return SDL_Color{ 0x0a,0xfd,0x04 };
	else if (money >= 1000000000 && money < 10000000000ui64)
		return SDL_Color{ 0xad,0x10,0x10 };
	else
		return SDL_Color{ 0x00,0x00,0xff };
}

SDL_Color Color::GetPlayerNameColor(uint8_t loop)
{
	if (loop == 0)
		return SDL_Color{ 0,255,0 ,255 };
	else if (loop == 1)
		return SDL_Color{ 250,120,20,255 };
	else if (loop == 1)
		return SDL_Color{ 0,255,255 ,255 };
	else
		return SDL_Color{ 178,34,34 , 255 };
}

