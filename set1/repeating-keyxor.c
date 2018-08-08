#include <stdio.h>
#include <string.h>
#include <stdlib.h>


unsigned char *xor(char *s, char *key)
{
  int i,k;
  char *o = (char *) malloc(strlen(s)+1);
  k = 0;
  for (i = 0; i < strlen(s); i++) {
    sprintf(o+i*2, "%02x", s[i] ^ key[k]);
    k++;
    if (k == strlen(key))
      k = 0;
  }
  o[i] = '\0';
  printf("\n");
  return o;
}


int main(int argc, char *argv[])
{
  int i;
  char *s = "Burning 'em, if you ain't quick and nimble\
             \nI go crazy when I hear a cymbal";
  char *email = "<html>elej</html>";
  char *key = "ICE";
  unsigned char *o = xor(s, key);
  printf("s: %s\ns^key: %s\n", s, o);
  printf("mail: %s\nemail^key: %s\n", email, xor(email, key));
  return 0;
}
