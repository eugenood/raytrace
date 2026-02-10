#include "image.hpp"
#include "interval.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

Color& Color::operator+=(const Color& color)
{
    r += color.r;
    g += color.g;
    b += color.b;

    return *this;
}

Color operator+(const Color& color1, const Color& color2)
{
    return Color{color1.r + color2.r, color1.g + color2.g, color1.b + color2.b};
}

Color operator*(const Color& color1, const Color& color2)
{
    return Color{color1.r * color2.r, color1.g * color2.g, color1.b * color2.b};
}

Color operator*(const Color& color, double factor)
{
    return Color{color.r * factor, color.g * factor, color.b * factor};
}

Color operator*(double factor, const Color& color)
{
    return Color{factor * color.r, factor * color.g, factor * color.b};
}

Color operator/(const Color& color, double factor)
{
    return Color{color.r / factor, color.g / factor, color.b / factor};
}

Color linearToGamma(const Color& color)
{
    return Color{std::sqrt(color.r), std::sqrt(color.g), std::sqrt(color.b)};
}

Image::Image(int width, int height)
    : m_width{width}
    , m_height{height}
    , m_pixels(static_cast<std::size_t>(width * height))
{
    // pass
}

int Image::width() const
{
    return m_width;
}

int Image::height() const
{
    return m_height;
}

Color& Image::operator()(int x, int y)
{
    return m_pixels[static_cast<std::size_t>(x + m_width * y)];
}

const Color& Image::operator()(int x, int y) const
{
    return m_pixels[static_cast<std::size_t>(x + m_width * y)];
}

bool saveImageAsBitmap(const Image& image, const std::string& filename)
{
    std::ofstream file{filename, std::ios::binary};

    if (!file)
        return false;

    int width{image.width()};
    int height{image.height()};
    int paddingSize{(4 - (width * 3) % 4) % 4};
    int scanlineSize{(width * 3) + paddingSize};
    int imageSize{scanlineSize * height};

    BitmapHeader header{};
    header.fileSize = sizeof(header) + static_cast<std::uint32_t>(imageSize);
    header.width = static_cast<std::uint16_t>(width);
    header.height = static_cast<std::uint16_t>(height);
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    static const std::array<std::uint8_t, 3> padding{0, 0, 0};
    static const Interval colorRange{0, 0.999};

    for (int y{0}; y < height; ++y)
    {
        for (int x{0}; x < width; ++x)
        {

            const Color& color{linearToGamma(image(x, y))};
            std::array<std::uint8_t, 3> pixel{
                static_cast<std::uint8_t>(clamp(colorRange, color.b) * 256.0),
                static_cast<std::uint8_t>(clamp(colorRange, color.g) * 256.0),
                static_cast<std::uint8_t>(clamp(colorRange, color.r) * 256.0),
            };
            file.write(reinterpret_cast<const char*>(pixel.begin()), 3);
        }

        if (paddingSize > 0)
            file.write(reinterpret_cast<const char*>(padding.begin()), paddingSize);
    }

    return true;
}