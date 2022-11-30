#include "MyCharImage.h"
#include "MyFloatImage.h"
#include "math.h"
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>

const double PI = 3.1415926535897932384626433832795;


CMyCharImage::CMyCharImage(void)
{
}

CMyCharImage::~CMyCharImage(void)
{
}

bool
CMyCharImage::CalcHisto(CMyHisto& histo, int channel)
{
  histo.Reset();
  if ((channel < 0) || (channel >= m_depth))
    return false;

  int size = m_width * m_height;

  unsigned char* p = m_pData + channel;
  for (int i = 0; i < size; i++, p+=m_depth)
    histo.Increment(*p);

  return true;
}

void
CMyCharImage::WriteBmpHeader(ostream& os, short int bbp, int offset, int sizeOfFile,
                                  int sizeOfImage) const
{
  int i;
  short int si;
  unsigned char ch;

  // ************************************ header = 14 byte ****************************
  
  // ASCII-Text BM = 2 bytes
  ch = 0x42;
  os.write((char*)&ch, 1);
  ch = 0x4D;
  os.write((char*)&ch, 1);
 
  // SizeOfFile
  os.write((char*)&sizeOfFile, 4);

  // Further Use, must be 0
  i = 0;
  os.write((char*)&i, 4);

  // Offset bis Bitmap in bytes = Header + BitmapInfo
  os.write((char*)&offset, 4);

  /************************* bitmap information = 44 bytes *************************/
  
  // number of bytes in header = 40
  i = 40;
  os.write((char*)&i, 4);

  // width and height in pixel
  os.write((char*)&m_width, 4);
  os.write((char*)&m_height, 4);
  
  // number of planes, must be 1
  si = 1;
  os.write((char*)&si, 2);
  // bits per pixel;
  os.write((char*)&bbp, 2);
  // compression 0 = no compression
  i = 0;
  os.write((char*)&i, 4);
  // size of image in bytes
  os.write((char*)&sizeOfImage, 4);
  // resolution, must be 0
  i = 0;
  os.write((char*)&i, 4); 
  os.write((char*)&i, 4); 
  // number of color indizes used and important
  i = 0; //256;
  os.write((char*)&i, 4); 
  os.write((char*)&i, 4);
}

bool
CMyCharImage::ReadBmpHeader(istream& is, short int& bpp, int& width, int& height,
                                 int& palEntries) const
{
  int i, offset;
  unsigned char ch;
  short int si;

  // ************************************ header = 14 byte ****************************

  // ASCII-text BM = 2 bytes
  is.read((char*)&ch, 1);
  if (ch != 'B')
    return false;
  is.read((char*)&ch, 1);
  if (ch != 'M')
    return false;

  // size of file in bytes = header + bitmapInfo + palett entries + bitmap
  is.read((char*)&i, 4);
  // further use, must be 0
  is.read((char*)&i, 4);
  // offset till bitmap starts in bytes = header + bitmapInfo + palett entries
  is.read((char*)&offset, 4);

  /************************* Bitmap Information = 44 bytes *************************/
  
  // number of bytes in header (=40)
  is.read((char*)&i, 4);
  if ( i != 40 )
    return false;

  // width and height in pixel
  is.read((char*)&width, 4);
  is.read((char*)&height, 4);
  // number of planes, must be 1
  is.read((char*)&si, 2);
  if (si != 1)
    return false;
  
  // bits per pixel = 8;
  is.read((char*)&bpp, 2);
  // compression 0 = no compression
  is.read((char*)&i, 4);
  if (i != 0)
    return false;
  // size of image in bytes
  is.read((char*)&i, 4);
  
  // resolution, must be 0
  is.read((char*)&i, 4); 
  is.read((char*)&i, 4); 
  // number of color indizes used und important
  is.read((char*)&i, 4); 
  is.read((char*)&i, 4);

  palEntries = (offset - 54);

  return ((bpp == 8) || (bpp == 24));
}

bool
CMyCharImage::ReadBmpFile(const char* fileName)
{
  ifstream is(fileName, ios::binary);
  if (!is)
    return false;

  int width, height, palEntries;
  short int bpp;
  if (!ReadBmpHeader(is, bpp, width, height, palEntries))
    return false;   

  unsigned char ch;
  unsigned char pixel;
  int i;

  if (bpp == 8)  // skip pal
  {
    for (i = 0; i < palEntries; i++)
      is.read((char*)&ch, 1);
  }

  Resize(width, height, bpp / 8);

  // increase no of bytes per line such that resulting number can be divided by 4
  int rest = (m_width*sizeof(unsigned char)*m_depth)%4;
  if (rest != 0)
    rest = 4 - rest;

  unsigned char* p;
  int x,y,d,line;
  for (y = 0; y < m_height; y++ )
  {
    line = m_height - y - 1;
    p = m_pData + line*m_width*m_depth;
    for (x = 0; x < m_width; x++)
    {
      for (d = m_depth - 1; d >= 0; d--)
      {
        is.read((char*)&ch, 1);
        pixel = ch;
        *(p + d) = pixel;
      }
      p += m_depth;
    }
    for (i = 0; i < rest; i++)
    {
      is.read((char*)&ch, 1);
    }
  }

  return true;
}


bool
CMyCharImage::WriteBmpFile(const char* fileName) const
{
  if (m_width * m_height * m_depth <= 0)
    return false;

  if (m_pData == NULL)
    return false;

  if ((m_depth != 1) && (m_depth != 3))
    return false;

  /*  just for template classes
  if (typeid(T) != typeid(unsigned char))
    return false;
  */

  short int bpp;
  if (m_depth == 1)
    bpp = 8;
  if (m_depth == 3)
    bpp = 24;

  ofstream os(fileName, ios::binary);
  if (!os)
    return false;

  int i, x, y, d;
  unsigned char pixel = 0;
  unsigned char pixeldummy = 0;
  char ch;
  const unsigned char* p = m_pData;

  // increase no of bytes per line such that resulting number can be divided by 4
  int bytesProZeile = m_width*sizeof(unsigned char)*m_depth;
  int rest = bytesProZeile%4;
  if (rest != 0)
  {
    rest = 4 - rest;
    bytesProZeile += rest;
  }

  int offset; 
  int sizeOfFile;
  int sizeOfImage;
  if (bpp == 8)
  {
    // offset in bytes = Header + BitmapInfo + Palette
    offset      = 1078;  // = 14 + 44 + 1024
  }
  else
  {
    // offset in bytes = Header + BitmapInfo
    offset      = 54;  // = 14 + 44
  }
  // size of file in bytes = Header + BitmapInfo + Bitmap
  sizeOfFile  = offset + m_height * bytesProZeile;
  // size of image in bytes
  sizeOfImage = m_height * bytesProZeile;

  WriteBmpHeader(os, bpp, offset, sizeOfFile, sizeOfImage);

  // ************************* palette entries = 1024 bytes *********************
 
  if (bpp == 8)
  {
    pixeldummy = 0;
    for (int j = 0; j < 256; j++)
    {
      ch = j;
      os.write((char*)&ch, 1);
      os.write((char*)&ch, 1);
      os.write((char*)&ch, 1);
      os.write((char*)&ch, 1);
    }
  }

  // ********* bitmap = height * bytesProZeile *********************************

  pixeldummy = 0;
  for (y = 0; y < m_height; y++)
  {
    p = m_pData + (m_height - y - 1)*m_width*m_depth;
    for (x = 0; x < m_width; x++)
    {
      for (d = m_depth - 1; d >= 0; d--)  // flip RGB to GBR
      {
        pixel = *(p+d);
        os.write((char*)&(pixel), 1);
      }     
      p += m_depth;
    }
    for (i = 0; i < rest; i++)
      os.write((char*)&pixeldummy, 1);
  }

  os.close();

  return true;
}

void
CMyCharImage::Convert(const CMyFloatImage& toCopy)
{
  if (m_pData != NULL)
  {
    free(m_pData);
    m_pData = NULL;
  }

  m_width  = toCopy.GetWidth();
  m_height = toCopy.GetHeight();
  m_depth  = toCopy.GetDepth();

  if (m_width * m_height * m_depth > 0)
  {
    m_pData = (unsigned char*) malloc(GetStorage());
    float               value;
    const float         *pSource = toCopy.GetData();
    unsigned char       *pDest = m_pData;
    int size = m_width * m_height * m_depth;
    for (int i = 0; i < size; i++)
    {
      value = *pSource++;
      if (value > 255)
        value = 255;
      if (value < 0)
        value = 0;
      *pDest++ = (unsigned char)value;
    }
  }
  else
    m_pData = NULL;
}

int
CMyCharImage::CalculateThreshByOtsu(int channel)
{
  CMyHisto histo;
  CalcHisto(histo, channel);

  if (histo.GetCount() == 0)
    return -1;

  double sigmaB    = 0.0;
  double maxSigmaB = -1.0;

  int T;
  int TStar = -1;

  double count= (double)histo.GetCount();
  double mue  = histo.CalcMean();
  double mueT = 0.0;
  double mueH = 0.0;
  double mueV = 0.0;
  double pH = 0.0;
  double hT = 0.0;
  for (T = 0; T < 255; T++)
  {
    hT = histo.GetEntry(T) / count;
    pH += hT;
    mueT += T*hT;
    if ((fabs(pH) > 1e-30) && (fabs(1-pH)>1e-30))
    {
      sigmaB = pow(mue*pH-mueT,2) / (pH*(1-pH));
      if (sigmaB > maxSigmaB)
      {
        maxSigmaB = sigmaB;
        TStar     = T;
      } 
    }
  }
  return TStar;
}

/**********************************************************************************/
/***************************** Aufgaben *******************************************/
/**********************************************************************************/

bool
CMyCharImage::DifferenceImage(const CMyCharImage& image1, const CMyCharImage& image2)
{
 /************** todo ****************/
  return true;
}


/**********************************************************************************/
/***************************** Tests **********************************************/
/**********************************************************************************/


bool
CMyCharImage::RGB2HSI(const CMyCharImage& source)
{
  if (source.m_depth != 3) return false;
  this->Resize(source.m_width, source.m_height, source.m_depth);

  unsigned char* destP = m_pData;
  unsigned char* end = source.GetData() + source.GetStorage();
  for (unsigned char* srcP = source.GetData(); srcP != end; srcP += 3, destP += 3) {
   double r = *(srcP + 0) / 255.0;
   double g = *(srcP + 1) / 255.0;
   double b = *(srcP + 2) / 255.0;

    double m = min(g, min(r, b));

    double h, s, i = 0.0;
    double denominator = sqrt(pow(r - g, 2) + (r - b) * (g - b));

    if (fabs(denominator) > numeric_limits<double>::epsilon()) {
      double phi = acos((0.5 * ((r - g) + (r - b))) / denominator);
      if (b <= g) {
        h = phi;
      } else {
        h = 2 * M_PI - phi;
      }
    }

    double rgbSum = r + g + b;
    if (fabs(rgbSum) > numeric_limits<double>::epsilon()) {
      s = 1.0 - 3.0 * (m / rgbSum);
    }

    i = rgbSum / 3.0;

    h *= 180 / (2 * M_PI);
    s *= 255;
    i *= 255;

    *(destP + 0) = std::clamp(h, 0.0, 255.0);
    *(destP + 1) = std::clamp(s, 0.0, 255.0);
    *(destP + 2) = std::clamp(i, 0.0, 255.0);
  }

  return true;
}

bool
CMyCharImage::HSI2RGB(const CMyCharImage& source)
{
  if (source.m_depth != 3) return false;
  this->Resize(source.m_width, source.m_height, source.m_depth);

  unsigned char* destP = m_pData;
  unsigned char* end = source.GetData() + source.GetStorage();
  for (unsigned char* srcP = source.GetData(); srcP != end; srcP += 3, destP += 3) {
    double h = *(srcP + 0) / 180.0 * 2.0 * M_PI;
    double s = *(srcP + 1) / 255.0;
    double i = *(srcP + 2) / 255.0;

    double r, g, b;

    if (0 <= h && h <= 2.0 / 3.0 * M_PI) {
      b = i * (1.0 - s);
      r = i * (1.0 + (s * cos(h)) / cos(M_PI / 3.0 - h));
      g = 3.0 * i - (r + b);
    } else if (2.0 / 3.0 * M_PI  < h && h <= 4.0 / 3.0 * M_PI) {
      h -= 2.0 / 3.0 * M_PI;
      r = i * (1.0 - s);
      g = i * (1.0 + (s * cos(h)) / cos(M_PI / 3.0 - h));
      b = 3.0 * i - (r + g);
    } else if (4.0 / 3.0 * M_PI < h && h <= 2.0 * M_PI) {
      h -= 4.0 / 3.0 * M_PI;
      g = i * (1.0 - s);
      b = i * (1.0 + s * cos(h) / cos(M_PI / 3.0 - h));
      r = 3.0 * i - (g + b);
    } else {
      r = g = b = 1;
      std::cout << "Error: h = " << h << std::endl;
    }

    r *= 255;
    g *= 255;
    b *= 255;

    *(destP + 0) = std::clamp(r, 0.0, 255.0);
    *(destP + 1) = std::clamp(g, 0.0, 255.0);
    *(destP + 2) = std::clamp(b, 0.0, 255.0);
  }

  return true;
}
