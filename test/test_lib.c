#include <stdint.h>

// reminder of calling convention:
// https://en.wikipedia.org/wiki/X86_calling_conventions#List_of_x86_calling_conventions
// RDI, RSI, RDX, RCX, R8, R9, [XYZ]MM0–7
int plus1(int *a) {
  *a = *a + 1;
  return *a; // return val not needed/tested at the moment
}

void inc(long *d, long *s) { *d = (*s) + 1; }

void sixparamadd(uint64_t *o0, uint64_t *o1, uint64_t *o2, uint64_t *i0,
                 uint64_t *i1, uint64_t *i2) {
  *o0 = *i0 + 1;
  *o1 = *i1 + 1;
  *o2 = *i2 + 1;
}

void array_width_three_add(uint64_t o[3], uint64_t i[3]) {
  *o++ = *i++ + 1;
  *o++ = *i++ + 1;
  *o = *i + 1;
}

void add_two_numbers(uint64_t *o, uint64_t *i0, uint64_t *i1) {
  *o = *i0 + *i1;
}

void array_width_three_mul(uint64_t o[3], uint64_t i0[3], uint64_t i1[3]) {
  o[0] = i0[0] * i1[0];
  o[1] = i0[1] * i1[1];
  o[2] = i0[2] * i1[2];
  /** *o++ = *i0++ * *i1++; */
  /** *o++ = *i0++ * *i1++; */
  /** *o = *i0 * *i1; */
}

void array_width_three_add_pair(uint64_t o[3], uint64_t i0[3], uint64_t i1[3]) {
  o[0] = i0[0] + i1[0];
  o[1] = i0[1] + i1[1];
  o[2] = i0[2] + i1[2];
}

void array_width_five_add_pair(uint64_t o[5], uint64_t i0[5], uint64_t i1[5]) {
  o[0] = i0[0] + i1[0];
  o[1] = i0[1] + i1[1];
  o[2] = i0[2] + i1[2];
  o[3] = i0[3] + i1[3];
  o[4] = i0[4] + i1[4];
}

typedef unsigned __int128 fiat_25519_uint128;
typedef unsigned char fiat_25519_uint1;
void fiat_25519_carry_mul(uint64_t out1[5], const uint64_t arg1[5],
                          const uint64_t arg2[5]) {
  fiat_25519_uint128 x1;
  fiat_25519_uint128 x2;
  fiat_25519_uint128 x3;
  fiat_25519_uint128 x4;
  fiat_25519_uint128 x5;
  fiat_25519_uint128 x6;
  fiat_25519_uint128 x7;

  fiat_25519_uint128 x8;
  fiat_25519_uint128 x9;
  fiat_25519_uint128 x10;
  fiat_25519_uint128 x11;
  fiat_25519_uint128 x12;
  fiat_25519_uint128 x13;
  fiat_25519_uint128 x14;
  fiat_25519_uint128 x15;
  fiat_25519_uint128 x16;
  fiat_25519_uint128 x17;
  fiat_25519_uint128 x18;
  fiat_25519_uint128 x19;
  fiat_25519_uint128 x20;
  fiat_25519_uint128 x21;
  fiat_25519_uint128 x22;
  fiat_25519_uint128 x23;
  fiat_25519_uint128 x24;
  fiat_25519_uint128 x25;
  fiat_25519_uint128 x26;
  uint64_t x27;
  uint64_t x28;
  fiat_25519_uint128 x29;
  fiat_25519_uint128 x30;
  fiat_25519_uint128 x31;
  fiat_25519_uint128 x32;
  fiat_25519_uint128 x33;
  uint64_t x34;
  uint64_t x35;
  fiat_25519_uint128 x36;
  uint64_t x37;
  uint64_t x38;
  fiat_25519_uint128 x39;
  uint64_t x40;
  uint64_t x41;
  fiat_25519_uint128 x42;
  uint64_t x43;
  uint64_t x44;
  uint64_t x45;
  uint64_t x46;
  uint64_t x47;
  uint64_t x48;
  uint64_t x49;
  fiat_25519_uint1 x50;
  uint64_t x51;
  uint64_t x52;
  x1 = ((fiat_25519_uint128)(arg1[4]) * ((arg2[4]) * UINT8_C(0x13)));
  x2 = ((fiat_25519_uint128)(arg1[4]) * ((arg2[3]) * UINT8_C(0x13)));
  x3 = ((fiat_25519_uint128)(arg1[4]) * ((arg2[2]) * UINT8_C(0x13)));
  x4 = ((fiat_25519_uint128)(arg1[4]) * ((arg2[1]) * UINT8_C(0x13)));
  x5 = ((fiat_25519_uint128)(arg1[3]) * ((arg2[4]) * UINT8_C(0x13)));
  x6 = ((fiat_25519_uint128)(arg1[3]) * ((arg2[3]) * UINT8_C(0x13)));
  x7 = ((fiat_25519_uint128)(arg1[3]) * ((arg2[2]) * UINT8_C(0x13)));
  x8 = ((fiat_25519_uint128)(arg1[2]) * ((arg2[4]) * UINT8_C(0x13)));
  x9 = ((fiat_25519_uint128)(arg1[2]) * ((arg2[3]) * UINT8_C(0x13)));
  x10 = ((fiat_25519_uint128)(arg1[1]) * ((arg2[4]) * UINT8_C(0x13)));
  x11 = ((fiat_25519_uint128)(arg1[4]) * (arg2[0]));
  x12 = ((fiat_25519_uint128)(arg1[3]) * (arg2[1]));
  x13 = ((fiat_25519_uint128)(arg1[3]) * (arg2[0]));
  x14 = ((fiat_25519_uint128)(arg1[2]) * (arg2[2]));
  x15 = ((fiat_25519_uint128)(arg1[2]) * (arg2[1]));
  x16 = ((fiat_25519_uint128)(arg1[2]) * (arg2[0]));
  x17 = ((fiat_25519_uint128)(arg1[1]) * (arg2[3]));
  x18 = ((fiat_25519_uint128)(arg1[1]) * (arg2[2]));
  x19 = ((fiat_25519_uint128)(arg1[1]) * (arg2[1]));
  x20 = ((fiat_25519_uint128)(arg1[1]) * (arg2[0]));
  x21 = ((fiat_25519_uint128)(arg1[0]) * (arg2[4]));
  x22 = ((fiat_25519_uint128)(arg1[0]) * (arg2[3]));
  x23 = ((fiat_25519_uint128)(arg1[0]) * (arg2[2]));
  x24 = ((fiat_25519_uint128)(arg1[0]) * (arg2[1]));
  x25 = ((fiat_25519_uint128)(arg1[0]) * (arg2[0]));
  x26 = (x25 + (x10 + (x9 + (x7 + x4))));
  x27 = (uint64_t)(x26 >> 51);
  x28 = (uint64_t)(x26 & UINT64_C(0x7ffffffffffff));
  x29 = (x21 + (x17 + (x14 + (x12 + x11))));
  x30 = (x22 + (x18 + (x15 + (x13 + x1))));
  x31 = (x23 + (x19 + (x16 + (x5 + x2))));
  x32 = (x24 + (x20 + (x8 + (x6 + x3))));
  x33 = (x27 + x32);
  x34 = (uint64_t)(x33 >> 51);
  x35 = (uint64_t)(x33 & UINT64_C(0x7ffffffffffff));
  x36 = (x34 + x31);
  x37 = (uint64_t)(x36 >> 51);
  x38 = (uint64_t)(x36 & UINT64_C(0x7ffffffffffff));
  x39 = (x37 + x30);
  x40 = (uint64_t)(x39 >> 51);
  x41 = (uint64_t)(x39 & UINT64_C(0x7ffffffffffff));
  x42 = (x40 + x29);
  x43 = (uint64_t)(x42 >> 51);
  x44 = (uint64_t)(x42 & UINT64_C(0x7ffffffffffff));
  x45 = (x43 * UINT8_C(0x13));
  x46 = (x28 + x45);
  x47 = (x46 >> 51);
  x48 = (x46 & UINT64_C(0x7ffffffffffff));
  x49 = (x47 + x35);
  x50 = (fiat_25519_uint1)(x49 >> 51);
  x51 = (x49 & UINT64_C(0x7ffffffffffff));
  x52 = (x50 + x38);
  out1[0] = x48;
  out1[1] = x51;
  out1[2] = x52;
  out1[3] = x41;
  out1[4] = x44;
}
