#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>

class Image
{
private:
    int width, height;
    std::vector<std::vector<uint32_t>> pixels; // 2D array for pixel data

public:
    // Constructor
    Image(int w, int h) : width(w), height(h), pixels(h, std::vector<uint32_t>(w, 0)) {}

    // Fill a rectangle
    void fillRect(int x, int y, int w, int h, uint32_t color)
    {
        for (int i = y; i < y + h; ++i)
        {
            for (int j = x; j < x + w; ++j)
            {
                if (i >= 0 && i < height && j >= 0 && j < width)
                {
                    pixels[i][j] = color;
                }
            }
        }
    }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Draw a line (Bresenham's algorithm)
    void drawLine(int x0, int y0, int x1, int y1, uint32_t color)
    {
        int dx = abs(x1 - x0), dy = abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true)
        {
            if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height)
            {
                pixels[y0][x0] = color;
            }
            if (x0 == x1 && y0 == y1)
                break;
            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }
    void drawIcon(const std::vector<uint8_t> &icon, int iconWidth, int iconHeight, int x, int y, uint32_t color)
    {
        for (int i = 0; i < iconHeight; ++i)
        { // Loop over rows
            for (int j = 0; j < iconWidth; ++j)
            {                                  // Loop over columns
                int index = i * iconWidth + j; // Calculate the 1D index in the icon data
                if (icon[index] != 0)
                {                                  // Only draw "on" pixels (non-zero values)
                    setPixel(x + j, y + i, color); // Set the corresponding pixel in the image
                }
            }
        }
    }

    // Draw a rectangle (empty)
    void drawRect(int x, int y, int w, int h, uint32_t color)
    {
        drawLine(x, y, x + w - 1, y, color);                 // Top edge
        drawLine(x, y, x, y + h - 1, color);                 // Left edge
        drawLine(x + w - 1, y, x + w - 1, y + h - 1, color); // Right edge
        drawLine(x, y + h - 1, x + w - 1, y + h - 1, color); // Bottom edge
    }

    // Set a single pixel color
    void setPixel(int x, int y, uint32_t color)
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            pixels[y][x] = color;
        }
    }

    // Get a single pixel color
    uint32_t getPixel(int x, int y) const
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            return pixels[y][x];
        }
        return 0; // Default to black if out of bounds
    }

    // Fill entire image with a color
    void fill(uint32_t color)
    {
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                pixels[i][j] = color;
            }
        }
    }

    // Clone the image
    Image clone() const
    {
        Image copy(width, height);
        copy.pixels = pixels;
        return copy;
    }

    // Flip horizontally
    void flipX()
    {
        for (int i = 0; i < height; ++i)
        {
            std::reverse(pixels[i].begin(), pixels[i].end());
        }
    }

    // Flip vertically
    void flipY()
    {
        std::reverse(pixels.begin(), pixels.end());
    }

    // Scroll (translate pixels)
    void scroll(int dx, int dy)
    {
        std::vector<std::vector<uint32_t>> newPixels(height, std::vector<uint32_t>(width, 0));
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                int newX = j + dx;
                int newY = i + dy;
                if (newX >= 0 && newX < width && newY >= 0 && newY < height)
                {
                    newPixels[newY][newX] = pixels[i][j];
                }
            }
        }
        pixels = newPixels;
    }

    // Replace one color with another
    void replace(uint32_t from, uint32_t to)
    {
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                if (pixels[i][j] == from)
                {
                    pixels[i][j] = to;
                }
            }
        }
    }

    // Check if two images are equal
    bool equals(const Image &other) const
    {
        return width == other.width && height == other.height && pixels == other.pixels;
    }
};
