#include "MyFilter.h"
#include <stdlib.h> 


CMyFilter::CMyFilter(void)
{
  m_width  = 0;
  m_height = 0;
  m_scale  = 1.0;
  m_pData  = NULL;
  m_resultType = UDEF_FILTER;
}

CMyFilter::~CMyFilter(void)
{
  if (m_pData != NULL)
    free(m_pData);
}

bool CMyFilter::InitMeanFilter(int width, int height)
{
  m_resultType = UCHAR_FILTER;

  if (m_pData != NULL)
    free(m_pData);

  int size = width * height;
  if (size < 1)
    return false;

  m_width  = width;
  m_height = height;

  m_pData  = (int*) malloc(m_width*m_height*sizeof(int));

  int* p   = m_pData;
  for (int i = 0; i < size; i++)
    *p++ = 1;
  m_scale  = 1.0 / size;

  return true;
}

void CMyFilter::InitSobelX()
{
/************** todo ****************/
}

void CMyFilter::InitSobelY()
{
/************** todo ****************/
}
