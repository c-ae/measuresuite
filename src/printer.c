#include "printer.h"
#include <stdio.h>
void printbin_single_number(uint64_t n) {

  int width = 64; // bits
  uint64_t mask = (uint64_t)1 << 63;
  while (width--) {
    if (n & mask)
      printf("1");
    else
      printf("0");

    mask >>= 1;
  }
  printf("\n");
}

void printbin(uint64_t *a, uint64_t *b, int len, int l) {
  int color = 32 - (*a != *b); // col 32 when its equal, grn
  printf("\n\x1b[%dm @ %d/%d: \n"
         "a:0b",
         color, len, l);
  printbin_single_number(*a);
  printf("b:0b");
  printbin_single_number(*b);
  printf("\x1b[0m");
}

void printhex(uint64_t *a, uint64_t *b, int len, int l) {
  int color = 32 - (*a != *b); // col 32 when its equal, grn
  printf("\n\x1b[%dm @ %d/%d: \n"
         "a:0x%016lx\n"
         "b:0x%016lx\x1b[0m\n",
         color, len, l, *a, *b);
}
