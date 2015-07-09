#include "hmiworks.h"

extern int ladder_dnk_ldx(int);


BYTE ladder_main(BYTE en)
{
  ladder_dnk_ldx(en);
  return 1;
}

