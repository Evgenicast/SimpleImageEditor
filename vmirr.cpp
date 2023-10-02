#include "ppm_image.h"

#include <algorithm>
#include <iostream>
#include <string_view>

using namespace std;

// реализуйте вертикальное отражение
void VMirrInplace(img_lib::Image & image)
{
    int w = image.GetWidth();
    int h = image.GetHeight();

    //std::swap_ranges(image.GetPixel())
    std::vector<img_lib::Color> buff(w * 3);


    for (int i = 0;  i < h/2; ++i)
    {
        img_lib::Color* line1 = image.GetLine(i);
        img_lib::Color* line2 = image.GetLine(h - i - 1);
        std::swap_ranges(line1, line1 + w, line2);
    }

};

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
        return 1;
    }

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    VMirrInplace(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}