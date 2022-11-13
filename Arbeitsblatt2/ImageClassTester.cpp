// ImageClassTester.cpp : Definiert den Einstiegspunkt f?r die Konsolenanwendung.
//

#include "MyImage.h"
#include <cstring>
#include <iostream>

#ifdef __linux__
std::string IMG_PATH = "/home/uncreative/Git/BVLab/Bilddaten_1_DigitaleBilder/";
#else
std::string IMG_PATH = std::getenv("IMAGES");
#endif

int main(int argc, char *argv[])
{
    string images[7] = {"Flower.bmp", "Kap.bmp", "Pedestrians.bmp", "SinglePedestrian.bmp", "Zellen1.bmp", "Zellen2.bmp", "Zellen3.bmp"};
    CMyImage testImage;
    for (auto & image : images) {
        // Absolute path
        testImage.ReadBmpFile((IMG_PATH + image).c_str());
        double otsu = testImage.CalcThreshByOtsu();
        cout << image << ": " << otsu << endl;
    }
    return 0;
}

