#include <vector>
#include <memory>

class Buffer; // Placeholder for Buffer class

class Image
{
private:
    int32_t width, height;
    std::vector<std::vector<int32_t>> pixels; // 2D array for pixel data

public:
    // Constructor
    Image(int32_t w, int32_t h);

    // Properties
    int32_t getWidth() const;
    int32_t getHeight() const;
    bool isMono() const;

    // Methods
    void copyFrom(const Image &from);
    void setPixel(int32_t x, int32_t y, int32_t c);
    int32_t getPixel(int32_t x, int32_t y) const;
    void fill(int32_t c);
    void getRows(int32_t x, Buffer &dst) const;
    void setRows(int32_t x, const Buffer &src);
    std::shared_ptr<Image> clone() const;
    void flipX();
    void flipY();
    std::shared_ptr<Image> transposed() const;
    void scroll(int32_t dx, int32_t dy);
    std::shared_ptr<Image> doubledX() const;
    std::shared_ptr<Image> doubledY() const;
    void replace(int32_t from, int32_t to);
    std::shared_ptr<Image> doubled() const;
    void drawImage(const Image &from, int32_t x, int32_t y);
    void drawTransparentImage(const Image &from, int32_t x, int32_t y);
    bool overlapsWith(const Image &other, int32_t x, int32_t y) const;
};

// Namespace for image-related functions
namespace image
{
    std::shared_ptr<Image> create(int32_t width, int32_t height);
    std::shared_ptr<Image> ofBuffer(const Buffer &buf);
    std::shared_ptr<Buffer> doubledIcon(const Buffer &icon);
}
