#pragma once

class CMyFilter
{
  // size of filter
  int    m_width;
  int    m_height;
  // weights
  int*   m_pData;
  // scaling
  double m_scale;
  // resulting data type
  int    m_resultType;

public:
  // possible types for the result
  typedef enum{UDEF_FILTER = 0, UCHAR_FILTER = 1, INT_FILTER = 2, FLOAT_FILTER = 3};

  CMyFilter(void);
  ~CMyFilter(void);

  // getter
  int GetWidth() const { return m_width; };
  int GetHeight() const { return m_height; };
  int GetResultType() const { return m_resultType; };
  int GetEntry(int x, int y) const { return m_pData[y*m_width + x];};
  const int* GetData() const { return m_pData; };
  double GetScale() const { return m_scale; };
 
  // initialize filter as meanfilter with given size
  bool InitMeanFilter(int width, int height);

  //......... todo .........

  // initialize as sobel filter in x-direction
  void InitSobelX();
  // initialize as sobel filter in y-direction
  void InitSobelY();
};
