#include "ppm_image.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string_view>

using namespace std;

int Sum(img_lib::Color c)
{
    return (to_integer<int>(c.r) + to_integer<int>(c.g) + to_integer<int>(c.b));  // /3
}

// реализуйте оператор Собеля
img_lib::Image Sobel(const img_lib::Image & image)
{
    int w = image.GetWidth();
    int h = image.GetHeight();

    auto ImageDst(image);

    img_lib::Color * line = ImageDst.GetLine(0);
    img_lib::Color * line2 = ImageDst.GetLine(h-1);
    std::fill(line, line + w, img_lib::Color::Black());  // top
    std::fill(line2, line2 + w, img_lib::Color::Black());  // bottom

    // left

    for (int y = 1; y < h-1; ++y)
    {
        img_lib::Color * line = ImageDst.GetLine(y);
        line[0] = img_lib::Color::Black();             // left
        line[w-1] = img_lib::Color::Black();           // right
        //
    }

    for (int y = 1; y < h - 1; ++y )
    {
        const img_lib::Color * line1 = image.GetLine(y-1);
        const img_lib::Color * line2 = image.GetLine(y);
        const img_lib::Color * line3 = image.GetLine(y+1);

        img_lib::Color * line2_dst = ImageDst.GetLine(y);

        for (int x = 1; x < w - 1; ++x)
        {
            // gx = −tl − 2tc − tr + bl + 2bc + br,
            int gx = -Sum(line1[x-1]) - 2*Sum(line1[x]) - Sum(line1[x+1]) +
                      Sum(line3[x-1]) + 2*Sum(line3[x]) + Sum(line3[x+1]);
//            cout << "gx = " << gx << endl;

            // gy = −tl − 2cl − bl + tr + 2cr + br.
            int gy = -Sum(line1[x-1]) - 2*Sum(line2[x-1]) - Sum(line3[x-1]) +
                      Sum(line1[x+1]) + 2*Sum(line2[x+1]) + Sum(line3[x+1]);
//            cout << "gy = " << gy << endl;

            double Sobelfilt = sqrt(pow(gx,2) + pow(gy,2));

//            cout << Sobelfilt << endl;

//            auto ClampedSobel = std::clamp(Sobelfilt, 0.0, 255.0);
            auto ClampedSobel = std::clamp<double>(Sobelfilt, 0, 255);
            line2_dst[x].r = static_cast<byte>(ClampedSobel);
            line2_dst[x].g = static_cast<byte>(ClampedSobel);
            line2_dst[x].b = static_cast<byte>(ClampedSobel);

        }
    }

    return ImageDst;
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

    image = Sobel(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}
