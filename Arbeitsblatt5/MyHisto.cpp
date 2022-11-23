#define _AFXDLL
#include "MyHisto.h"
#include <math.h>

CMyHisto::CMyHisto(void)
{
  Reset();
  m_count = 0;
}

CMyHisto::~CMyHisto(void)
{
}

void CMyHisto::Reset()
{
  m_count = 0;
  for (int i = 0; i < HISTO_SIZE; i++)
    m_data[i] = 0;
}

void CMyHisto::Increment(unsigned char value)
{
  m_count++;
  m_data[value]++;
}

bool CMyHisto::WriteHisto(const char* fileName)
{
  ofstream os(fileName);
  if (!os)
    return false;

  for (int i = 0; i < HISTO_SIZE; i++)
  {
    os << i << "\t" << m_data[i] << endl;
  }
  os << "\t" << m_count << endl; 
  os.close();
  return true;
}

bool CMyHisto::WriteNormalizedHisto(const char* fileName, double factor)
{
  ofstream os(fileName);
  if (!os)
    return false;
  if (m_count == 0)
    return false;

  double count = m_count;
  for (int i = 0; i < HISTO_SIZE; i++)
  {
    os << i << "\t" << factor * m_data[i] / count << endl;
  }
  os << "\t" << m_count << endl; 
  os.close();
  return true;
}

double CMyHisto::CalcMean()
{
  if (m_count == 0)
    return -1.0;

  double mean = 0.0;
  for (int i = 0; i < HISTO_SIZE; i++)
  {
    mean += i * (double)m_data[i];
  }
  return mean / m_count;
}

double CMyHisto::CalcSigma(double mean)
{
  if (m_count == 0)
    return -1.0;

  double sigma = 0.0;
  for (int i = 0; i < HISTO_SIZE; i++)
  {
    sigma += pow(i - mean,2) * (double)m_data[i];
  }
  return sqrt(sigma / m_count);
}

double CMyHisto::CalcVariance(double mean)
{
  if (m_count == 0)
    return -1.0;

  double sigma = 0.0;
  for (int i = 0; i < HISTO_SIZE; i++)
  {
    sigma += pow(i - mean,2) * (double)m_data[i];
  }
  return (sigma / m_count);
}

