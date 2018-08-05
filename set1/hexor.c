#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hex2base64.h"

// uncoment it to test
/*
char *hex2xor(char *, char *, int);

int main(int argc, int *argv[])
{
  char *s1 = "1c0111001f010100061a024b53535009181c";
  char *s2 = "686974207468652062756c6c277320657965";

  printf("%s\n", hex2xor(s1, s2, strlen(s1)));
}

*/

char *hex2xor(char *s1, char *s2, int len)
{
  int i;
  char *o = malloc(strlen(s1)+1);
  for (i = 0; i < len; i++)
    sprintf(o+i, "%x", nibbletoi(s1[i]) ^ nibbletoi(s2[i]));
  return o;
}
