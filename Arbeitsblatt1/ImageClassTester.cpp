// ImageClassTester.cpp : Definiert den Einstiegspunkt f?r die Konsolenanwendung.
//

#include "MyImage.h"
#include "MyHisto.h"
#include <time.h>
#include <iostream>

int main(int argc, char *argv[])
{
    CMyImage testImage;
    // Absolute path
    testImage.ReadBmpFile("/home/uncreative/Git/BVLab/Bilddaten_1_DigitaleBilder/Kap.bmp");

    // Slow way
    clock_t start_slow, finish_slow;
    start_slow = clock();

    for (int x = 0; x < testImage.GetWidth(); x++) {
        for (int y = 0; y < testImage.GetHeight(); y++) {
            testImage.SetPixel(x, y, 255 - testImage.GetPixel(x, y));
        }
    }

    finish_slow = clock();

    testImage.WriteBmpFile("/home/uncreative/Git/BVLab/Bilddaten_1_DigitaleBilder/Kap_new.bmp");
    std::cout << "Run time slow method: " << (double) (finish_slow -start_slow) / CLOCKS_PER_SEC << std::endl;

     // Absolute path
    testImage.ReadBmpFile("/home/uncreative/Git/BVLab/Bilddaten_1_DigitaleBilder/Kap.bmp");

    // Slow way
    clock_t start_fast, finish_fast;
    start_fast = clock();

    unsigned char* p = testImage.m_pData;

    for (int y = 0; y < testImage.GetHeight(); y++) {
        p = testImage.m_pData + (testImage.GetHeight() - y - 1)*testImage.GetWidth();
        for (int x = 0; x < testImage.GetHeight(); x++) {
            *p = 255 - *p;
            p++;
        }
    }

    finish_fast = clock();

    testImage.WriteBmpFile("/home/uncreative/Git/BVLab/Bilddaten_1_DigitaleBilder/Kap_new.bmp");
    std::cout << "Run time fast method: " << (double) (finish_fast -start_fast) / CLOCKS_PER_SEC << std::endl;


    return 0;
}

