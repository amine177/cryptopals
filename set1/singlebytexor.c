#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hex2base64.h"


struct bytescore {
  unsigned char byte;
  int score;
  int firstindex;
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
      ptr->firstindex = i / 2;
      o[k++] = *ptr;
    }
  }


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

unsigned char analyse(char *s)
{
  unsigned char key = 0x0, res;
  int max, len, sscore;
  char *decoded = malloc(strlen(s)/2+1);
  struct bytescore *o = 
    (struct bytescore *) malloc(sizeof(struct bytescore) * strlen(s)/2);
  len = score(s, o);
  res = 0x0;
  max = 0;
  while (key != 0xff) {
    sscore = 0;
    for (int i = 0; i < len; i++)
      if (isalpha(o[i].byte^key))
        sscore += o[i].score;
    if (sscore > max) {
      max = sscore;
      res = key;
    }
    key++;
  }

  return res;
  /*
  printf("Analysing againsg etaoinshrdlu as frequent\
      english chars and the space...\n");
  char *letter = "etaoinshdrluk ";
  for (int x = 0; x < 2; x++) {
    printf("analysing against: %s\n", letter);
    printf("\n\n");
    for (int i = 0; i < strlen(letter); i++) {
      key = o[0].byte ^ letter[i];
      printf("key: %x, byte: %d, index: %d\n", key, o[0].byte, o[0].firstindex);
      for (int k = 0; k < strlen(s); k+=2) {
        byte = nibbletoi(*(s+k)) * 16 + nibbletoi(*(s+k+1));
        char c = byte ^ key;
        if (c == letter[i] && k/2 == o[0].firstindex)
          alphan = 1;
        decoded[k/2] = byte ^ key;
      }
      if (alphan)
        printf("potential solution(%d):\n%s\n", alphan, decoded);
      alphan = 0;
      printf("\n");
      printf("\n\n");
    }
    letter = "ETAOINSHDRLUK ";
  }
  */
}

int main(int argc, char *argv[])
{
  char *s = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  unsigned char key = analyse(s);
  printf("Trying to resolve:\n");
  for (int i = 0; i < strlen(s); i+=2) {
    printf("%c", (nibbletoi(s[i])*16 + nibbletoi(s[i+1]))^key );
  }
  printf("\n");

  return 0;
}
