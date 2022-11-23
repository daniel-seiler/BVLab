#include "MyFloatImage.h"
#include "MyCharImage.h"
#include <math.h>

CMyFloatImage::CMyFloatImage(void)
{
}

CMyFloatImage::~CMyFloatImage(void)
{
}

void
CMyFloatImage::Convert(const CMyCharImage& toCopy)
{
  if (m_pData != NULL)
    free(m_pData);

  m_width  = toCopy.GetWidth();
  m_height = toCopy.GetHeight();
  m_depth  = toCopy.GetDepth();

  if (m_width * m_height * m_depth > 0)
  {
    m_pData = (float*) malloc(GetStorage());
    const unsigned char *pSource = toCopy.GetData();
    float               *pDest = m_pData;
    int size = m_width * m_height * m_depth;
    for (int i = 0; i < size; i++)
      *pDest++ = (float)*pSource++;
  }
  else
    m_pData = NULL;
}

void
CMyFloatImage::ApplyAbs()
{
  int size = m_width * m_height * m_depth;
  float* p = m_pData;
  for (int i = 0; i < size; i++, p++)
  {
    *p = fabs(*p);
  }
}
