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
char *hex2base64(char *, int);
unsigned char bytetoi(unsigned char);
char *string2hex(char *, int);
char *base64tohex(char *);

int main(int argc, char *argv[])
{
  char *str = "0123ehllosd idsofjpsdfoijwepodfjsfopadjfpsaodijfoajfadpoifjapofij";
  char *hex = string2hex(str, strlen(str));
  char *base64 = hex2base64(hex, strlen(hex));
  printf("str: %s\n", str);
  printf("hex: %s\n", hex);
  printf("base64: %s\n", base64);
  printf("hex again: %s\n", base64tohex(base64));
}


char *string2hex(char *s, int len)
{
  char *o = malloc(len*2+1);
  int i;
  for (i = 0; i<len; i++)
    sprintf(o+i*2, "%02x", *(s+i));
  return o;
}

unsigned char nibbletoi(char c)
{
  if (c >= 'a' && c <= 'f') {
    return  c - 'a' + 10;
  }
  else if (c >= '0' && c <= '9')
    return c - '0';
}

char inttohexchar(unsigned char i)
{
  if (i >= 0 && i <= 9)
    return '0' + i;
  else if (i >= 10 && i <= 15)
    return 'a' + (i - 10);
}

unsigned char reversebase64(unsigned char c)
{
  if (c >= 'A' && c <= 'Z')
    return c - 'A';
  if (c >= 'a' && c <= 'z')
    return 26 + (c - 'a');
  if (c >= '0' && c <= '9')
    return 50 + (c - '0');
  if (c == '+')
    return 62;
  if (c == '/')
    return 63;
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
    *(c+i) = nibbletoi(s[i]);
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

char *base64tohex(char *s)
{
  int i, pad, j, olen;
  unsigned char *o;
  long unit;
  pad = 0;
  if (strstr(s, "==") != NULL)
    pad++;
  if (strstr(s, "=") != NULL)
    pad++;
  olen = ((strlen(s) * 6) - pad * 8) / 4;
  o = (unsigned char *) malloc(olen);
  j = 0;
  for (i = 0; i < strlen(s)-pad;) {
    if (s[i+2] != '=') {
      if (s[i+3] != '=') {
        unit = reversebase64(s[i]) << 18 | reversebase64(s[i+1]) << 12 
          | reversebase64(s[i+2]) << 6 | reversebase64(s[i+3]) & 0x3f;
        o[j]    = (unit & 0xF00000) >> 20;
        o[j+1]  = (unit & 0xF0000) >> 16;
        o[j+2]  = (unit & 0xF000) >> 12;
        o[j+3]  = (unit & 0xF00) >> 8;
        o[j+4]  = (unit & 0xF0) >> 4;
        o[j+5]  = (unit & 0xF);
        j+= 6;
        i+= 4;
      } else {
        unit = reversebase64(s[i]) << 10 | reversebase64(s[i+1]) << 4
          | (reversebase64(s[i+2]) >> 2);
        o[j] = (unit & 0xF000) >>  12;
        o[j+1] = (unit & 0xF00) >> 8;
        o[j+2] = (unit & 0xF0) >> 4;
        o[j+3] = (unit & 0xF);
        j += 4;
        i += 3;
        break;
      }

    } else {
      unit = reversebase64(s[i]) << 2 | (reversebase64(s[i+1]) >> 4);
      o[j] = (unit & 0xF0) >> 4;
      o[j+1] = (unit & 0xF);
      j += 2;
      i += 2;
      break;
    }
  }
  for (i = 0; i < olen; i++)
    o[i] = inttohexchar(o[i]);
  o[olen] = '\0';

  return o;
}
