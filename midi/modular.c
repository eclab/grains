#include "modular.h"

#define RESERVED_CC 0
#define AUXILIARY_CC 9
#define SYSTEM_CC 10
#define MODULATION_CC 11
#define BAD 255

#define S(x, y) (x * 16 + y)
#define SS(x) S(x, x)

const PROGMEM unsigned char CC_FUNCS[64] = 
    {
    SS(SYSTEM_CC),                  // 0/32
    SS(SYSTEM_CC),
    SS(AUXILIARY_CC),
    SS(1),
    SS(1),
    SS(SYSTEM_CC),
    SS(RESERVED_CC),
    SS(RESERVED_CC),
    SS(RESERVED_CC),
    SS(RESERVED_CC),
    SS(6),
    SS(6),
    SS(2),
    SS(2),
    SS(3),
    SS(3),
    SS(4),
    SS(4),
    SS(5),
    SS(5),
    SS(6),
    SS(6),
    SS(7),
    SS(7),
    SS(8),
    SS(8),
    SS(MODULATION_CC),
    SS(MODULATION_CC),
    SS(RESERVED_CC),
    SS(RESERVED_CC),
    SS(RESERVED_CC),
    SS(RESERVED_CC),
    S(SYSTEM_CC, SYSTEM_CC),                // 64/96
    S(1, SYSTEM_CC),
    S(1, SYSTEM_CC),
    S(1, SYSTEM_CC),
    S(1, SYSTEM_CC),
    S(1, SYSTEM_CC),
    S(2, 7),
    S(2, 7),
    S(2, 7),
    S(2, 7),
    S(SYSTEM_CC, 7),
    S(2, 8),
    S(3, 8),
    S(3, 8),
    S(3, 8),
    S(3, 8),
    S(3, MODULATION_CC),
    S(4, MODULATION_CC),
    S(4, MODULATION_CC),
    S(4, MODULATION_CC),
    S(4, MODULATION_CC),
    S(4, MODULATION_CC),
    S(5, RESERVED_CC),
    S(5, RESERVED_CC),
    S(5, SYSTEM_CC),
    S(5, SYSTEM_CC),
    S(5, RESERVED_CC),
    S(6, SYSTEM_CC),
    S(6, RESERVED_CC),
    S(6, RESERVED_CC),
    S(6, RESERVED_CC),
    S(6, RESERVED_CC)
    };
        
unsigned int getModCCFunction(unsigned char cc)
    {
    if (cc < 64)
        {
        if (cc < 32) return CC_FUNCS[cc] >> 4;
        else return CC_FUNCS[cc - 32] & 15;
        }
    else if (cc < 96) return CC_FUNCS[cc - 32] >> 4;
    else return CC_FUNCS[cc - 64] & 15;
    }

unsigned char getModCCParameter(unsigned char cc)
    {
    if (cc < 6) return cc - 4;
    if (cc < 14) return cc - 12;
    if (cc < 26) 
        {
        return (cc - 14) & 1;
        }
    if (cc < 28) return cc - 26;
        
    if (cc < 58)
        {
        unsigned char val = getModCCParameter(cc - 32);
        if (val == 0) return 8;
        else return 7;
        }
        
    if (cc < 60) return BAD;
    if (cc < 74) return cc - 70 + 2;
    if (cc == 75) return 6;
    if (cc < 81) return cc - 76 + 2;
    if (cc < 86) return cc - 80 + 2;
    if (cc < 91) return cc - 86 + 2;
    if (cc < 96) return cc - 91 + 1;
    if (cc < 107) return cc - 102 + 2;
    if (cc < 112) return cc - 86 + 2;
    }
        
/*

  unsigned int getFunction(unsigned char cc)
  {
  if (cc == 3 || cc == 35) return AUXILIARY_CC;
  else if (cc >= 65 && cc <= 111)
  {
  if (cc >= 102)
  {
  if (cc >= 107) return 8;
  else return 7;
  }
  else if (cc <= 73)
  {
  if (cc >= 70) return 2;
  else return 1;
  }
  else if (cc >= 75 && cc <= 95)
  {
  if (cc >= 86)
  {
  if (cc >= 91) return 6;
  else return 5;
  }
  else
  {
  if (cc >= 81) return 4;
  else if (cc >= 76) return 3;
  else return 2;
  }
  }
  else return SYSTEM_CC;
  }
  else if (cc >= 12 && cc <= 25)
  {
  }
  if (cc >= 12 + 32 && cc <= 25 + 32)
  {
  }
        
  if (cc <= 64 && cc 
  if (cc >= 102 && cc <= 111)
  {
  }
  }
*/
        
unsigned int getParameter(unsigned char cc, unsigned char id)
    {
    }
        
