#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hex2base64.h"


struct bytescore {
  unsigned char byte;
  int score;
};

struct bytescorer {
  struct bytescore *ptr;
  unsigned char (*next)(struct bytescore *, int, int);
  int occ;
  int len;
};

int score(char *s, struct bytescore *o)
{
  int i,j, k, found;
  void sort(struct bytescore ptr[], int);
  unsigned char byte;
  struct bytescore *ptr = (struct bytescore *) malloc(sizeof(struct bytescore));

  k = 0;
  found = 0;
  for (i = 0; i < strlen(s); i+=2) {
    found = 0;
    byte = nibbletoi(*(s+i)) * 16 + nibbletoi(*(s+i+1));
    for (j = 0; j < k; j++)
      if (o[j].byte == byte) {
        o[j].score++;
        found = 1;
        break;
      }
    if (!found) {
      ptr->byte = byte;
      ptr->score = 1;
      o[k++] = *ptr;
    }
  }

  sort(o, k);

  return k;
}


void sort(struct bytescore ptr[], int len)
{
  int swap = 1, i;
  struct bytescore temp;

  while (swap) {
    swap = 0;
    for (i = 0; i < len - 1; i++) {
      if (ptr[i].score < ptr[i+1].score) {
        temp = ptr[i];
        ptr[i] = ptr[i+1];
        ptr[i+1] = temp;
        swap = 1;
      }
    }
  }
}

void analyse(char *s)
{
  unsigned char key, byte;
  struct bytescore *o = 
    (struct bytescore *) malloc(sizeof(struct bytescore) * strlen(s)/2);
  int len = score(s, o);
  printf("Analysing againsg etaoinshrdlu as frequent\
      english chars and the space...\n");
  char *letter = "etaoinshdrluk ";
  for (int x = 0; x < 2; x++) {
    printf("analysing against: %s\n", letter);
    printf("\n\n");
    for (int i = 0; i < strlen(letter); i++) {
      key = o[0].byte ^ letter[i];
      printf("key: %x, byte: %d\n", key, o[0].byte);
      for (int k = 0; k < strlen(s); k+=2) {
        byte = nibbletoi(*(s+k)) * 16 + nibbletoi(*(s+k+1));
        printf("%c", byte ^ key);
      }
      printf("\n");
      printf("\n\n");
    }
    letter = "ETAOINSHDRLUK ";
  }
}

int main(int argc, char *argv[])
{
  char *s = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  analyse(s);
  getchar();
}
