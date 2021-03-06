////////////////////////////////////////////////////////////////
//                                                            //
//  DO NOT EDIT THIS FILE -- WRITE YOUR FUNCTIONS IN  tp2.c   //
//                                                            //
////////////////////////////////////////////////////////////////

// Reads the contents of "tp2.c", ignoring any main() function,
#define main no__main
#include "tp2.c"
#undef main

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

struct xx_ranctx { uint32_t a; uint32_t b; uint32_t c; uint32_t d; } xx_ranc;

uint32_t xx_ranval()
{
#define rot(x, k) (((x) << (k)) | ((x) >> (32 - (k))))
  uint32_t e = xx_ranc.a - rot(xx_ranc.b, 27);
  xx_ranc.a = xx_ranc.b ^ rot(xx_ranc.c, 17);
  xx_ranc.b = xx_ranc.c + xx_ranc.d;
  xx_ranc.c = xx_ranc.d + e;
  xx_ranc.d = e + xx_ranc.a;
  return xx_ranc.d;
}

void xx_raninit(uint32_t seed)
{
  xx_ranc.a = 0xf1ea5eed, xx_ranc.b = xx_ranc.c = xx_ranc.d = seed;
  for (uint32_t i = 0; i < 20; ++i)
    (void) xx_ranval();
}

FILE* xx_trace;
const char* xx_trace_name;

void xx_trace_init(const char* name)
{
  xx_trace_name = getenv("OUTPUT");
  if (!xx_trace_name)
    {
      static char tmp[30];
      strncpy(tmp, name, 23);
      strcat(tmp, ".trace");
      xx_trace_name = tmp;
    }
  xx_trace = fopen(xx_trace_name, "w+");
  if (!xx_trace)
    {
      fprintf(stderr, "cannot open '%s' for writing: %s\n",
	      xx_trace_name, strerror(errno));
      exit(2);
    }

  time_t rawtime = time(NULL);
  struct tm *timeinfo = gmtime(&rawtime);
  char buffer[20];
  strftime(buffer, 80, "%F %T%n", timeinfo);

  int u = getuid();
  int r = rawtime & 0xffffff;
  fprintf(xx_trace, "%s\n%d\n%d\n", name, u, r);
  fputs(buffer, xx_trace);
  u ^= r;
  const char* s = getenv("SEED");
  if (s)
    u = atoi(s);
  xx_raninit(u);
}

void xx_trace_done()
{
  fclose(xx_trace);
  printf("please submit '%s'\n", xx_trace_name);
}

int xx_cmp_count = 0;

int xx_cmp(int a, int b)
{
  ++xx_cmp_count;
  if (((a&1) == 0) && ((b&1) == 1))
    return -1;
  if (((a&1) == 1) && ((b&1) == 0))
    return 1;
  if (a < b)
    return -1;
  return a > b;
}

int xx_cmpp(const void* a, const void* b)
{
  return xx_cmp(*(const int*)a, *(const int*)b);
}

int xx_lt(int a, int b)
{
  ++xx_cmp_count;
  if (a < b)
    return -1;
  return a > b;
}

int xx_gt(int a, int b)
{
  return xx_lt(b, a);
}

int xx_ltp(const void* a, const void* b)
{
  return xx_lt(*(const int*)a, *(const int*)b);
}

int xx_gtp(const void* a, const void* b)
{
  return xx_lt(*(const int*)b, *(const int*)a);
}

void xx_fisher_yates_shuffle(int* tab, unsigned count)
{
  unsigned i = count;
  while (i)
    {
      unsigned j = xx_ranval() % i--;
      int tmp = tab[i];
      tab[i] = tab[j];
      tab[j] = tmp;
    }
}

int main()
{
#if (Q == 1) || (Q == 2)
#  if Q == 1
  xx_trace_init("tp2q1");
  fprintf(xx_trace, "linear_search(..., 0, ..., ...) == %u\n",
	  linear_search(0, 0, 0, 0));
#  else
  xx_trace_init("tp2q2");
  fprintf(xx_trace, "binary_search(..., 0, ..., ...) == %u\n",
	  binary_search(0, 0, 0, 0));
#  endif
  const unsigned asize = (1U << 17) - 1;
  int a[asize];
  fprintf(xx_trace, "initializing a[%u] = { ... }\n", asize);
  for (unsigned i = 0; i < asize; ++i)
    a[i] = (int) xx_ranval();
  int b[13];
  int c[13];
  int d[13];
  memcpy(b, a, sizeof(b));
  b[10] = -2147483647 - 1;
  b[11] = 0;
  b[12] = 2147483647;
  fprintf(xx_trace, "set b[] of values to lookup\n");
  print_int_array(xx_trace, b, 13);
  qsort(a, asize, sizeof(*a), xx_cmpp);
  for (unsigned i = 0; i < 13; ++i)
    {
      xx_cmp_count = 0;
#  if Q == 1
      c[i] = linear_search(a, asize, b[i], xx_cmp);
#  else
      c[i] = binary_search(a, asize, b[i], xx_cmp);
#  endif
      d[i] = xx_cmp_count;
    }
  fprintf(xx_trace, "resulting indices in a[]\n");
  print_int_array(xx_trace, c, 13);
  fprintf(xx_trace, "number of comparisons in each case\n");
  print_int_array(xx_trace, d, 13);
  xx_trace_done();
#elif Q == 3
  xx_trace_init("tp2q3");
  const unsigned asize = 80 + (xx_ranval() % 32);
  int a[asize];
  fprintf(xx_trace, "initializing a[%u] = { ... }\n", asize);
  for (unsigned i = 0; i < asize; ++i)
    a[i] = (int) xx_ranval();
  xx_cmp_count = 0;
  bs_insert_sort_cmp(a, asize, xx_cmp);
  fprintf(xx_trace,
	  "bs_insert_sort_cmp(a,...) made %d comparisons\n",
	  xx_cmp_count);
  print_int_array(xx_trace, a, asize);
  xx_trace_done();
#elif Q == 4
  xx_trace_init("tp2q4");
  const unsigned asize = 100000;
  int a[asize];
  fprintf(xx_trace, "initializing a[%u] = { ... }\n", asize);
  int val = -1000000;
  for (unsigned i = 0; i < asize; ++i)
    a[i] = val += 1 + (xx_ranval() % 40);
  int a2[asize];
  memcpy(a2, a, sizeof(a));
  qsort(a2, asize, sizeof(*a), xx_gtp);
  int b[13];
  for (unsigned i = 0; i < 10; ++i)
    b[i] = (xx_ranval() % 2000000) - 1000000;
  b[10] = -2147483647 - 1;
  b[11] = 0;
  b[12] = 2147483647;
  fprintf(xx_trace, "set b[] of values to lookup\n");
  print_int_array(xx_trace, b, 13);
  int c[13];
  int c2[13];
  for (unsigned i = 0; i < 13; ++i)
    {
      c[i] = interpolate(a, 0, asize - 1, b[i], xx_lt);
      c2[i] = interpolate(a2, 0, asize - 1, b[i], xx_gt);
    }
  fprintf(xx_trace, "resulting indices in a[]\n");
  print_int_array(xx_trace, c, 13);
  fprintf(xx_trace, "resulting indices in a2[]\n");
  print_int_array(xx_trace, c2, 13);
  xx_trace_done();
#elif Q == 5
  xx_trace_init("tp2q5");
  fprintf(xx_trace, "interpolation_search(..., 0, ..., ...) == %u\n",
	  interpolation_search(0, 0, 0, 0));
  const unsigned asize = 100000;
  int a[asize];
  fprintf(xx_trace, "initializing a[%u] = { ... }\n", asize);
  int val = -1000000;
  for (unsigned i = 0; i < asize; ++i)
    a[i] = val += 1 + (xx_ranval() % 40);
  int a2[asize];
  memcpy(a2, a, sizeof(a));
  qsort(a2, asize, sizeof(*a), xx_gtp);
  int b[13];
  for (unsigned i = 0; i < 10; ++i)
    b[i] = (xx_ranval() % 2000000) - 1000000;
  b[10] = -2147483647 - 1;
  b[11] = 0;
  b[12] = 2147483647;
  fprintf(xx_trace, "set b[] of values to lookup\n");
  print_int_array(xx_trace, b, 13);
  int c[13];
  int c2[13];
  for (unsigned i = 0; i < 13; ++i)
    {
      c[i] = interpolation_search(a, asize, b[i], xx_lt);
      c2[i] = interpolation_search(a2, asize, b[i], xx_gt);
    }
  fprintf(xx_trace, "resulting indices in a[]\n");
  print_int_array(xx_trace, c, 13);
  fprintf(xx_trace, "resulting indices in a2[]\n");
  print_int_array(xx_trace, c2, 13);
  xx_trace_done();
#else
  puts("Compile with -DQ=n, where n is the question number.");
#endif
  return 0;
}
