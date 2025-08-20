#pragma once
#include <string>
#include <SDL2/SDL.h>

class Color
{
public:

    /**
     * Constructor. Initializes the color to black.
     */
    Color();

    /**
     * Constructs a color from the bytes in an integer. Call it with
     * a hexadecimal constant for HTML-style color representation.
     * The alpha component will be set to 255.
     *
     * EXAMPLE: Color(0xff50a0) constructs Gui-chan's favourite color.
     *
     * NOTE: Because of this constructor, integers will be automatically
     *       casted to a color by your compiler.
     *
     * @param color the color.
     */
    Color(int color);

    /**
     * Constructor.
     *
     * @param r Red color component (range 0-255).
     * @param g Green color component  (range 0-255).
     * @param b Blue color component (range 0-255).
     * @param a Color alpha, used for transparency. A value of 0 means
     *          totaly transparent, 255 is totaly opaque (the default).
     */
    Color(int r, int g, int b, int a = 255);

    /**
     * Adds the RGB values of two colors together. The values will be
     * clamped if they go out of range.
     *
     * @param color a color to add to this color.
     * @return the resulting color with alpha set to 255.
     */
    Color operator+(const Color& color) const;

    /**
     * Subtracts the RGB values of one color from another.
     * The values will be clamped if they go out of range.
     *
     * @param color a color to subtract from this color.
     * @return the resulting color with alpha set to 255.
     */
    Color operator-(const Color& color) const;

    /**
     * Multiplies the RGB values of a color with a float value.
     * The values will be clamped if they go out of range.
     *
     * @param value the value to multiply the color with.
     * @return the resulting color with alpha untouched.
     */
    Color operator*(float value) const;

    /**
     * Compares two colors.
     *
     * @return true if the two colors have the same RGBA components.
     */
    bool operator==(const Color& color) const;

    /**
     * Compares two colors.
     *
     * @return true if the two colors have different RGBA components.
     */
    bool operator!=(const Color& color) const;

    operator SDL_Color() const;

    /**
     * Red color component (range 0-255).
     */
    int r;

    /**
     * Green color component (range 0-255).
     */
    int g;

    /**
     * Blue color component (range 0-255).
     */
    int b;

    /**
     * Color alpha, used for transparency. A value of 0 means totaly
     * transparent, 255 is totaly opaque (the default)
     */
    int a;

    static void HexToColor(const std::wstring& hex, SDL_Color& c);

    static SDL_Color GetMoneyColor(uint64_t money);
    static SDL_Color GetPlayerNameColor(uint8_t loop);

    static SDL_Color RED;
    static SDL_Color GREEN;
    static SDL_Color BLUE;
    static SDL_Color BLACK;
    static SDL_Color PINK;
    static SDL_Color ORANGE;
    static SDL_Color YELLOW;
    static SDL_Color YANGRED;
    static SDL_Color PURPLE;
    static SDL_Color WHITE;
    static SDL_Color GRAY;
    static SDL_Color BUTTON;
    static SDL_Color XYPURPLE;
};