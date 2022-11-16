#include "MyTypeImage.h"


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
    return false;

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

  /************** todo ****************/
  
  return true;
}

template< class T >
bool
CMyTypeImage<T>::CopyChannel(const CMyTypeImage<T>& source, int channel)
{

  /************** todo ****************/

  return true;
}
