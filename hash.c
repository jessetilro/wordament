#include "hash.h"

unsigned int hash_polynomial(const char* key) {
  int n = strlen(key);
  uintmax_t hash_code = polynomial(n-1, 31, key, n);
  return (unsigned int) hash_code;
}

// Horner's rule for polynomial evaluation
uintmax_t polynomial(unsigned int p, const unsigned int z, const char* av, unsigned int n) {
  int idx = n - 1;
  if (p == 0) {
    return (uintmax_t) av[idx];
  }
  idx -= p;
  uintmax_t a = (uintmax_t) av[idx];
  return a + (z * polynomial(p - 1, z, av, n));
}
