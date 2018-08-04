#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXHEX 1000

static const char base64index[64] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '+', '/'
};

// for testing
/*
char *hex2base64(char *, int);
unsigned char bytetoi(unsigned char);
char *string2hex(char *, int);

int main(int argc, char *argv[])
{
  char *str = "0";
  char *hex = string2hex(str, strlen(str));
  char *base64 = hex2base64(hex, strlen(hex));
  printf("str: %s\n", str);
  printf("hex: %s\n", hex);
  printf("base64: %s\n", base64);
}


char *string2hex(char *s, int len)
{
  char *o = malloc(len*2+1);
  int i;
  for (i = 0; i<len; i++)
    sprintf(o+i*2, "%02x", *(s+i));
  return o;
}
*/

unsigned char bytetoi(unsigned char c)
{
  if (c >= 'a' && c <= 'f') {
    return  c - 'a' + 10;
  }
  else if (c >= '0' && c <= '9')
    return c - '0';
}

char *hex2base64(char *s, int len)
{
  /* = padding and output length in bits and chars and input length in bits */
  int pad, obits, olen, ibits, i, j;
  char *o;
  unsigned char *c;
  unsigned long unit, unitc;
  ibits = len * 4;
  switch (ibits % 6) {
    case 4:
      pad = 1;
      break;
    case 2 :
      pad = 2;
      break;
    default:
      pad = 0;
  }


  c = (unsigned char *) malloc(len + pad);
  obits = ibits + pad * 8;
  olen = obits / 6;
  o = (char *) malloc(olen+1);
  for (i = 0; i < strlen(s); i++) {
    *(c+i) = bytetoi(s[i]);
  }
  i = len;
  for ( j = 0; j < pad; j++)
    *(c+i+j) = 0;
  for (i = 0 , j = 0; i < len+pad; i+= 6, j += 4) {
    unit = c[i] << 20 | c[i+1] << 16 | c[i+2] << 12 | c[i+3] << 8 | c[i+4] << 4
      | c[i+5];
    o[j] = base64index[unit >> 18];
    o[j+1] = base64index[unit >> 12 & 0x3F];
    if ( i+2 < len-1 && i+3 < len-1) {
      o[j+2] = base64index[unit >> 6 & 0x3F];
      o[j+3] = base64index[unit & 0x3F];
    } else if (i+2 < len-1) {
        o[j+2] = base64index[unit >> 6 & 0x3F];
        o[j+3] = '=';
    } else {
        o[j+2] = '=';
        o[j+3] = '=';

    }
  }

  free(c);
  o[olen] = '\0';
  return o;
}
