#pragma once
#include "MyTypeImage.h"

class CMyIntImage :  public CMyTypeImage<int>
{
public:
  CMyIntImage(void);
  ~CMyIntImage(void);
};

