// unit-test
#include "../src/checker.h"

int zero_same_8b() {
  uint64_t a = 0;
  uint64_t b = 0;
  return check(1, &a, &b);
}

int deadbeef_same_4b() {
  uint64_t a = 0xDEADBEEF;
  uint64_t b = 0xDEADBEEF;
  return check(1, &a, &b);
}

int deadbeef_same_16b() {
  uint64_t a[] = {0xDEADBEEFDEADBEEF, 0xDEADBEEFDEADBEEF};
  uint64_t b[] = {0xDEADBEEFDEADBEEF, 0xDEADBEEFDEADBEEF};
  return check(2, a, b);
}

int deadbeef_diff_16b() {
  // both are different
  uint64_t a[] = {0xDEADBEEFDEADBEEF, 0xDEADBEEFDEADBEEF};
  uint64_t b[] = {0xDEADBEEFDEADBEF0, 0xDEADBEEFDEADBEF0};
  // -----------------------------^^----diff----------^^
  return check(2, a, b);
}

int deadbeef_diff_16b_1bdiff() {
  // both are different
  uint64_t a[] = {0xDEADBEEFDEADBEEF, 0xDEADBEEFDEADBEEF};
  uint64_t b[] = {0xDEADBEEFDEADBEEF, 0xDEADBEEFDEADBEF0};
  // ------------------------------diff---------------^^
  return check(2, a, b);
}

int deadbeef_diff_16b_2bdiff() {
  // both are different
  uint64_t a[] = {0xDEADBEEFDEADBEEF, 0xDEADBEEFDEADBEEF};
  uint64_t b[] = {0xDEADBEEFDEADBEF0, 0xDEADBEEFDEADBEEF};
  // -----------------------------^^-diff
  return check(2, a, b);
}

int main() {
  int res = 0;
  res |= zero_same_8b();
  res |= deadbeef_same_4b();
  res |= deadbeef_same_16b();
  res |= !deadbeef_diff_16b();
  res |= !deadbeef_diff_16b_1bdiff();
  res |= !deadbeef_diff_16b_2bdiff();
  return res;
}
