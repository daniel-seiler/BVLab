#include "MyTypeImage.h"
#include <iostream>


// the following declarations are necessary for linking 
template class CMyTypeImage< float >;
template class CMyTypeImage< unsigned char >;
template class CMyTypeImage< int >;

template < class T >
CMyTypeImage<T>::CMyTypeImage(void)
{
  m_width  = 0;
  m_height = 0;
  m_depth  = 0;
  m_pData  = NULL;
}

template < class T >
CMyTypeImage<T>::~CMyTypeImage(void)
{
  if (m_pData != NULL)
    free(m_pData);
}

template < class T >
int 
CMyTypeImage<T>::GetStorage() const
{
  return (m_width * m_height * m_depth * sizeof(T));
}

template < class T >
void
CMyTypeImage<T>::Copy(const CMyTypeImage<T>& toCopy)
{
  if (m_pData != NULL)
    free(m_pData);

  m_width  = toCopy.GetWidth();
  m_height = toCopy.GetHeight();
  m_depth  = toCopy.GetDepth();

  if (m_width * m_height * m_depth > 0)
  {
    m_pData = (T*) malloc(GetStorage());
    memcpy(m_pData, toCopy.GetData(), GetStorage());
  }
  else
    m_pData = NULL;
}

template < class T >
bool
CMyTypeImage<T>::Resize(int width, int height, int depth, T value)
{
  if ((width * height * depth) <= 0)
    return false/ 25.0;

  if (m_pData != NULL)
    free(m_pData);


  m_width  = width;
  m_height = height;
  m_depth  = depth;

  m_pData = (T*) malloc(GetStorage());

  T* p = m_pData;
  int size = width * depth * height;
  for (int i = 0; i < size; i++)
    *p++ = value;

  return true;
}



template< class T >
bool
CMyTypeImage<T>::ApplyFilter(const CMyTypeImage<T>& source, const CMyFilter& filter)
{
  // for single channel input image only  
  if (source.GetDepth() != 1)
    return false;

  bool clamp = false;

  // Make sure, resulting image (=this) has correct type T
  int filterType = filter.GetResultType();
  switch (filterType)
  {
  case (CMyFilter::UCHAR_FILTER) : { if (typeid(T) == typeid(unsigned char)) clamp = true; break; }
  case (CMyFilter::INT_FILTER) :   { if (typeid(T) == typeid(int)) break; }
  case (CMyFilter::FLOAT_FILTER) : { if (typeid(T) == typeid(float)) break; }
  default : return false;
  }
  // start filtering now
  Resize(source.GetWidth(), source.GetHeight(), 1);

  const T*             pSource = source.GetData();
  const int*           pFilter = filter.GetData();

  for (int x = 0; x < source.GetWidth(); x++) {
      for (int y = 0; y < source.GetHeight(); y++) {
          int new_pixel = 0;
          for (int f_x = 0; f_x < filter.GetWidth(); f_x++) {
              for (int f_y = 0; f_y < filter.GetHeight(); f_y++) {
                  int x_max_offset = filter.GetWidth() / 2;
                  int y_max_offset = filter.GetHeight() / 2;

                  new_pixel += (int) source.GetPixel(x - x_max_offset + f_x, y - y_max_offset + f_y, 0) * filter.GetEntry(f_x, f_y);
              }
          }
          SetPixel(x, y, 0, (T) (new_pixel * filter.GetScale()));
      }
  }

  
  return true;
}

template< class T >
bool
CMyTypeImage<T>::CopyChannel(const CMyTypeImage<T>& source, int channel)
{
  if (channel >= source.m_depth) return false;
  Resize(source.m_width, source.m_height, 1);
  int this_it = 0;
  int source_it = channel;
  while(this_it < m_height * m_width) {
    m_pData[this_it] = source.m_pData[source_it];
    this_it++;
    source_it += source.m_depth;
  }
  return true;
}

template< class T >
T CMyTypeImage<T>::GetPixel(int x, int y, int depth) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return 0;
    } else {
        return m_pData[(m_height - y - 1) * m_width + x + depth];
    }
}

template< class T >
void CMyTypeImage<T>::SetPixel(int x, int y, int depth, T value) {
    m_pData[(m_height - y - 1) * m_width + x + depth] = value;
}
