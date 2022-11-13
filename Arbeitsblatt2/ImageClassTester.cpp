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
    CMyImage testImage;
    // Absolute path
    testImage.ReadBmpFile((IMG_PATH + "Flower.bmp").c_str());
    double otsu = testImage.CalcThreshByOtsu();
    cout << otsu << endl;
    return 0;
}

