// ImageClassTester.cpp : Definiert den Einstiegspunkt f?r die Konsolenanwendung.
//

#include "MyImage.h"
#include "MyHisto.h"
#include <time.h>
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
    testImage.ReadBmpFile((IMG_PATH + "Kap.bmp").c_str());

    // Slow way
    clock_t start_slow, finish_slow;
    start_slow = clock();

    for (int x = 0; x < testImage.GetWidth(); x++) {
        for (int y = 0; y < testImage.GetHeight(); y++) {
            testImage.SetPixel(x, y, 255 - testImage.GetPixel(x, y));
        }
    }

    finish_slow = clock();

    testImage.WriteBmpFile((IMG_PATH + "Kap_new_1.bmp").c_str());
    std::cout << "Run time slow method: " << (double) (finish_slow -start_slow) / CLOCKS_PER_SEC << std::endl;

     // Absolute path
    testImage.ReadBmpFile((IMG_PATH + "Kap.bmp").c_str());

    // Slow way
    clock_t start_fast, finish_fast;
    start_fast = clock();

    for (unsigned char *p = testImage.m_pData; p < testImage.m_pData + testImage.GetWidth() * testImage.GetHeight(); p++) {
        *p = 255 - *p;
    }

    finish_fast = clock();

    testImage.WriteBmpFile(IMG_PATH.append("Kap_new_2.bmp").c_str());
    std::cout << "Run time fast method: " << (double) (finish_fast -start_fast) / CLOCKS_PER_SEC << std::endl;


    return 0;
}

