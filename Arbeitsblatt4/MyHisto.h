#pragma once

#include <fstream>
using namespace std;

class CMyHisto
{
public:
  static const int HISTO_SIZE = 256;

  int m_data[HISTO_SIZE];
  int m_count;

public:
  CMyHisto(void);
  ~CMyHisto(void);

  // initialize histogram with zero
  void Reset();

  // getter
  int GetCount(){ return m_count; };
  int GetEntry(int i){ return m_data[i]; };
  
  // increments histogram at position value
  void Increment(unsigned char value);
  
  // mean value
  double CalcMean();
  // standard deviation
  double CalcSigma(double mean);
  // variance
  double CalcVariance(double mean);

  // output to file - readable by excel
  bool WriteHisto(const char* fileName);
  bool WriteNormalizedHisto(const char* fileName, double factor = 1.0);
};
