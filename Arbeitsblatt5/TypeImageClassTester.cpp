// TypeImageClassTester.cpp : Definiert den Einstiegspunkt fuer die Konsolenanwendung.
//

#include "MyCharImage.h"
#include <iostream>

#ifdef __linux__
std::string IMG_PATH = "/home/uncreative/Git/BVLab/Bilddaten_5_RGB_HSI/";
#else
std::string IMG_PATH = std::getenv("IMAGES");
#endif

int main(int argc, char *argv[])
{
  CMyCharImage img = CMyCharImage();
  img.ReadBmpFile((IMG_PATH + "Spielzeug.bmp").c_str());

  CMyCharImage test1 = CMyCharImage();
  test1.RGB2HSI(img);
  CMyCharImage test2 = CMyCharImage();
  //test2.CopyChannel(test1, 2);
  test2.HSI2RGB(test1);
  test1.WriteBmpFile((IMG_PATH + "Spielzeug_HSI.bmp").c_str());
  //test2.CopyChannel(test1, 1);
  test2.HSI2RGB(test1);
  test2.WriteBmpFile((IMG_PATH + "Spielzeug_HSI2RGB.bmp").c_str());

  return 0;
}

