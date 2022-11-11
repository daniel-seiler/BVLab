#pragma once
#include "MyHisto.h"
#include <fstream>
#include <cstdint>
#include <cstring>
using namespace std;

class CMyImage
{
  int m_width;
  int m_height;


protected:

  int GetSizeInBytes() const {return (m_width * m_height * sizeof(unsigned char));};
  
  bool ReadBmpHeader(istream& is, int& width, int& height, int& palEntries) const;
  void WriteBmpHeader(ostream& os, int offset, int sizeOfFile, int sizeOfImage) const;
  
public:
  unsigned char* m_pData;

  CMyImage(void);
  CMyImage(const CMyImage& toCopy);
  
  ~CMyImage(void);

  int GetHeight() const {return m_height;};
  int GetWidth() const {return m_width;};

  const unsigned char* GetData() const {return m_pData;};
  unsigned char* GetData() {return m_pData;};

  bool ReadBmpFile(const char* fileName);
  bool WriteBmpFile(const char* fileName) const;

  // Aufgaben

  bool IsEmpty() const;

  void Copy(const CMyImage& toCopy);
  void Resize(int width, int height);

  void SetPixel(int x, int y, unsigned char value);
  unsigned char GetPixel(int x, int y) const;

  void Invert();

  void MakeBinary(int thresh);

  void CalcHisto(CMyHisto& histo) const;

  double CalcThreshByOtsu() const;
};
