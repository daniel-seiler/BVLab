#pragma once
#include "MyHisto.h"
#include "MyFilter.h"
#include <fstream>
#include <cstring>
using namespace std;

template< class T >
class CMyTypeImage
{
protected:

  // image width
  int m_width;
  
  // image height
  int m_height;
  
  // number of channels
  int m_depth;

  // image data
  T* m_pData;

  // storage in bytes
  int GetStorage() const;
    
public:
  CMyTypeImage(void);
  ~CMyTypeImage(void);

  // resize image
  bool Resize(int width, int height, int depth, T value = 0);
  // copy toCopy 
  void Copy(const CMyTypeImage<T>& toCopy);

  // getter
  int GetWidth()  const {return m_width;};
  int GetHeight() const {return m_height;};
  int GetDepth()  const {return m_depth;};
  T* GetData() const {return m_pData;};

  //......... todo .........
 
  bool CopyChannel(const CMyTypeImage<T>& source, int channel);

  bool ApplyFilter(const CMyTypeImage<T>& source, const CMyFilter& filter);
};


