#include "image.d.cpp"
#include <cstdint>
#include <string>
#include <vector>
#include <cmath> // for floor

// Placeholder for `TextEffectState`, which would be defined elsewhere
struct TextEffectState
{
    int xOffset; // Horizontal offset for text effects
    int yOffset; // Vertical offset for text effects
};

// Font structure definition
struct Font
{
    uint8_t charWidth;          // Character width
    uint8_t charHeight;         // Character height
    std::vector<uint64_t> data; // Glyph bitmap data
    uint8_t multiplier;         // Scaling factor (optional)
};

// Define font5
const Font font5 = {
    6, // Character width
    5, // Character height
    {
        0x2000000000000000, 0x2100001700000000, 0x2200000300030000, 0x23000A1F0A1F0A00,
        0x24000A17151D0A00, 0x2500130904121900, 0x26000A15150A1000, 0x2700000300000000,
        0x2800000E11000000, 0x290000110E000000, 0x2A00000A040A0000, 0x2B0000040E040000,
        0x2C00001008000000, 0x2D00000404040000, 0x2E00000800000000, 0x2F00100804020100,
        0x30000E11110E0000, 0x310000121F100000, 0x3200191515120000, 0x33000911150B0000,
        0x34000C0A091F0800, 0x3500171515150900, 0x3600081416150800, 0x3700110905030100,
        0x38000A1515150A00, 0x390002150D050200, 0x3A00000A00000000, 0x3B0000100A000000,
        0x3C0000040A110000, 0x3D00000A0A0A0000, 0x3E0000110A040000, 0x3F00020115050200,
        0x40000E1115090E00, 0x41001E05051E0000, 0x42001F15150A0000, 0x43000E1111110000,
        0x44001F11110E0000, 0x45001F1515110000, 0x46001F0505010000, 0x47000E1111150C00,
        0x48001F04041F0000, 0x4900111F11000000, 0x4A000911110F0100, 0x4B001F040A110000,
        0x4C001F1010100000, 0x4D001F0204021F00, 0x4E001F0204081F00, 0x4F000E11110E0000,
        0x50001F0505020000, 0x5100060919160000, 0x52001F05050A1000, 0x5300121515090000,
        0x540001011F010100, 0x55000F10100F0000, 0x5600070810080700, 0x57001F0804081F00,
        0x58001B04041B0000, 0x590001021C020100, 0x5A00191513110000, 0x5B00001F11110000,
        0x5C00010204081000, 0x5D000011111F0000, 0x5E00000201020000, 0x5F00101010101000,
        0x6000000102000000, 0x61000C12121E1000, 0x62001F1414080000, 0x63000C1212120000,
        0x64000814141F0000, 0x65000E1515120000, 0x6600041E05010000, 0x67000215150F0000,
        0x68001F0404180000, 0x6900001D00000000, 0x6A000010100D0000, 0x6B001F040A100000,
        0x6C00000F10100000, 0x6D001E0204021E00, 0x6E001E02021C0000, 0x6F000C12120C0000,
        0x70001E0A0A040000, 0x7100040A0A1E0000, 0x72001C0202020000, 0x730010140A020000,
        0x7400000F14141000, 0x75000E10101E1000, 0x7600060810080600, 0x77001E1008101E00,
        0x7800120C0C120000, 0x7900121408040200, 0x7A00121A16120000, 0x7B0000041F110000,
        0x7C00001F00000000, 0x7D00111F04000000, 0x7E00000404080800, 0xD3000C1213130C00,
        0xF3000C12130D0000, 0x04010E05051E1000, 0x05010609191F0800, 0x06010C1213131200,
        0x07010C1213130000, 0x18010F0B1B190000, 0x19010E151D1A0000, 0x41011F1412100000,
        0x4201100F14120000, 0x43011F0205081F00, 0x44011E03031C0000, 0x5A0110140B030200,
        0x5B0110140B030000, 0x7901121A17130000, 0x7A01121A17130000, 0x7B01121B17120000,
        0x7C01121B17120000},
    1 // Default multiplier
};

// Define font12 (placeholder)
const Font font12 = {
    12, // Character width
    12, // Character height
    {}, // Data (add actual font data here when available)
    1   // Default multiplier
};

// Function to get font based on text
const Font &getFontForText(const std::string &text)
{
    for (char c : text)
    {
        if (static_cast<uint32_t>(c) > 0x2000)
        {
            return font12;
        }
    }
    return font5;
}

// Function to scale a font
Font scaledFont(const Font &f, uint8_t size)
{
    if (screen::sleepmode)
    {
        return f
    }
    if (size < 2)
    {
        return f; // No scaling needed
    }

    Font scaled = f;
    scaled.charWidth = f.charWidth * size;
    scaled.charHeight = f.charHeight * size;
    scaled.multiplier = f.multiplier ? size * f.multiplier : size;
    return scaled;
}

// Function to double the font size
Font doubledFont(const Font &f)
{
    return scaledFont(f, 2);
}

// Function to print text centered
bool imagePrintCenter(Image &img, const std::string &text, int y, int color = -1, const Font *font = nullptr)
{
    if (screen::sleepmode)
    {
        return
    }

    if (!font)
    {
        font = &getFontForText(text);
    }

    int w = text.length() * font->charWidth;
    int x = std::floor((img.getWidth() - w) / 2.0);

    // Call the `imagePrint` function
    imagePrint(img, text, x, y, color, font);

    // Indicate success
    return true;
}

// Function to print text on an image
void imagePrint(
    Image &img,
    const std::string &text,
    int x,
    int y,
    int color = 1,
    const Font *font = nullptr,
    const std::vector<TextEffectState> *offsets = nullptr)
{

    x = std::floor(x);
    y = std::floor(y);
    if (screen::sleepmode)
    {
        return
    }
    if (!font)
    {
        font = &getFontForText(text);
    }

    int x0 = x;
    size_t cp = 0;
    int mult = font->multiplier ? font->multiplier : 1;

    int dataW = font->charWidth / mult;
    int dataH = font->charHeight / mult;

    int byteHeight = (dataH + 7) >> 3; // Divide by 8 and round up
    int charSize = byteHeight * dataW;
    int dataSize = 2 + charSize;

    const std::vector<uint64_t> &fontdata = font->data;
    int lastchar = static_cast<int>(fontdata.size() / dataSize) - 1;

    std::vector<uint8_t> imgBuf;
    if (mult == 1)
    {
        imgBuf.resize(8 + charSize);
        imgBuf[0] = 0x87;
        imgBuf[1] = 1;
        imgBuf[2] = dataW;
        imgBuf[4] = dataH;
    }

    while (cp < text.length())
    {
        int xOffset = 0, yOffset = 0;

        if (offsets && cp < offsets->size())
        {
            xOffset = (*offsets)[cp].xOffset;
            yOffset = (*offsets)[cp].yOffset;
        }

        char ch = text[cp++];
        if (ch == '\n')
        {
            y += font->charHeight + 2;
            x = x0;
            continue;
        }

        if (ch < 32)
        {
            continue;
        }

        int l = 0, r = lastchar, off = 0;
        int guess = (ch - 32) * dataSize;
        if (fontdata[guess / dataSize] == ch)
        {
            off = guess;
        }
        else
        {
            while (l <= r)
            {
                int m = l + ((r - l) >> 1);
                int v = fontdata[m * dataSize / sizeof(uint64_t)];
                if (v == ch)
                {
                    off = m * dataSize;
                    break;
                }
                if (v < ch)
                {
                    l = m + 1;
                }
                else
                {
                    r = m - 1;
                }
            }
        }

        if (mult == 1)
        {
            std::copy(fontdata.begin() + off + 2, fontdata.begin() + off + 2 + charSize, imgBuf.begin() + 8);
            img.drawIcon(imgBuf, dataW, dataH, x + xOffset, y + yOffset, color);
            x += font->charWidth;
        }
        else
        {
            off += 2;
            for (int i = 0; i < dataW; ++i)
            {
                int j = 0;
                uint8_t mask = 0x01;
                uint8_t c = fontdata[off++];
                while (j < dataH)
                {
                    if (mask == 0x100)
                    {
                        c = fontdata[off++];
                        mask = 0x01;
                    }
                    int n = 0;
                    while (c & mask)
                    {
                        n++;
                        mask <<= 1;
                    }
                    if (n)
                    {
                        img.fillRect(x + xOffset * mult, y + (j + yOffset) * mult, mult, mult * n, color);
                        j += n;
                    }
                    else
                    {
                        mask <<= 1;
                        j++;
                    }
                }
                x += mult;
            }
        }
    }
}
