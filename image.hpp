#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct Color
{
    double r{};
    double g{};
    double b{};
    Color& operator+=(const Color& color);
};

Color operator+(const Color& color1, const Color& color2);
Color operator*(const Color& color1, const Color& color2);
Color operator*(const Color& color, double factor);
Color operator*(double factor, const Color& color);
Color operator/(const Color& color, double factor);
Color linearToGamma(const Color& color);

class Image
{
public:
    Image(int width, int height);
    int width() const;
    int height() const;
    Color& operator()(int x, int y);
    const Color& operator()(int x, int y) const;

private:
    int m_width{};
    int m_height{};
    std::vector<Color> m_pixels{};
};

#pragma pack(push, 1)
struct BitmapHeader
{
    std::uint16_t fileType{0x4d42};
    std::uint32_t fileSize{};
    std::uint32_t reserved{0};
    std::uint32_t bitmapOffset{26};
    std::uint32_t size{12};
    std::uint16_t width{};
    std::uint16_t height{};
    std::uint16_t planes{1};
    std::uint16_t bitsPerPixel{24};
};
#pragma pack(pop)

bool saveImageAsBitmap(const Image& image, const std::string& filename);