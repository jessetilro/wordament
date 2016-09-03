#include <inttypes.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

unsigned int hash_polynomial(const char* key);
uintmax_t polynomial(unsigned int p, unsigned int z, const char* av, unsigned int n);
