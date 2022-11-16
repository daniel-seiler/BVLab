#pragma once
#include "MyTypeImage.h"

class CMyFloatImage;

class CMyCharImage :  public CMyTypeImage<unsigned char>
{
  void WriteBmpHeader(ostream& os, short int bbp, int offset, int sizeOfFile, int sizeOfImage) const;
  bool ReadBmpHeader(istream& is, short int& bpp, int& width, int& height, int& palEntries) const;
  
public:
  CMyCharImage(void);
  ~CMyCharImage(void);

  // calculate histogram for given channel
  // returns false if channel does not exist 
  bool CalcHisto(CMyHisto& histo, int channel = 0);

  // calculate thresh by otsu for given channel
  // returns -1 if the histogram for the given channel is empty
  int CalculateThreshByOtsu(int channel);
  
  // io for images with one or three channels
  // returns false in many cases... ;-)
  bool ReadBmpFile(const char* fileName);
  bool WriteBmpFile(const char* fileName) const;

  // copies float image with clamping 
  void Convert(const CMyFloatImage& toCopy);

  //......... todo .........

  // calculates difference image fabs(image1 - image2) for all channels
  // returns false if size (including number of channels) is of image1 and image2 not equal
  bool DifferenceImage(const CMyCharImage& image1, const CMyCharImage& image2);

  // apply given thresh to channel and return binary image
  // returns false if width * height * depth == 0 for source image
  // resulting image has only one channel with two values: 0 and 255
  // Attention: implementation has to be in the header-file
  template <class S>
  bool ApplyThresh(const CMyTypeImage<S>& source, double thresh, int channel);
  
  //......... todo later .........

  // convert from RGB to HSI and vice versa
  bool RGB2HSI(const CMyCharImage& source);
  bool HSI2RGB(const CMyCharImage& source);
};

template <class S>
bool
CMyCharImage::ApplyThresh(const CMyTypeImage<S>& source, double thresh, int channel)
{
  /************** todo ****************/

  return true;
}
