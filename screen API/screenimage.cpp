#include <memory>
#include <iostream>

// Forward declaration of the Image class
class Image;

// Global screen object (assuming it’s already initialized elsewhere)
std::shared_ptr<Image> screen = nullptr;

class Image
{
public:
    // Constructor
    Image()
    {
        // Initialize the screen image here (example placeholder)
        std::cout << "Screen image initialized." << std::endl;
    }

    // Additional functionalities for the Image class can go here
};

// Function to get the screen image
std::shared_ptr<Image> screenImage()
{
    if (!screen)
    {
        screen = std::make_shared<Image>();
    }
    return screen;
}