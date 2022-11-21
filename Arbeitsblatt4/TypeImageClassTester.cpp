// TypeImageClassTester.cpp : Definiert den Einstiegspunkt fuer die Konsolenanwendung.
//

#include "MyCharImage.h"
#include <iostream>

#ifdef __linux__
std::string IMG_PATH = "/home/uncreative/Git/BVLab/Bilddaten_4_Farbbilder/";
#else
std::string IMG_PATH = std::getenv("IMAGES");
#endif

int main(int argc, char *argv[])
{
  CMyCharImage img = CMyCharImage();
  img.ReadBmpFile((IMG_PATH + "Rose.bmp").c_str());

  CMyCharImage test = CMyCharImage();
  test.ApplyThresh(img, 240, 0);
  test.WriteBmpFile((IMG_PATH + "Rose_isolated_1.bmp").c_str());
  test = CMyCharImage();
  test.ApplyThresh(img, 40, 1);
  test.WriteBmpFile((IMG_PATH + "Rose_isolated_2.bmp").c_str());

  CMyFilter filter = CMyFilter();
  CMyCharImage source = CMyCharImage();
  source.CopyChannel(img, 1);
  filter.InitMeanFilter(5, 5);
  test = CMyCharImage();
  test.ApplyFilter(source, filter);
  test.WriteBmpFile((IMG_PATH + "Rose_filtered.bmp").c_str());

  return 0;
}

