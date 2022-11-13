// ImageClassTester.cpp : Definiert den Einstiegspunkt f?r die Konsolenanwendung.
//

#include "MyImage.h"
#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
    CMyImage testImage;
    // Absolute path
    testImage.ReadBmpFile(std::string(std::getenv("IMAGES")).append("/Flower.bmp").c_str());
    double otsu = testImage.CalcThreshByOtsu();
    cout << otsu << endl;
    return 0;
}

