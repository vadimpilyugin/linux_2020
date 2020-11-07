#include "buf.h"

#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>
#include <check.h>

static jmp_buf escape;

static void
test_abort(void)
{
    longjmp(escape, 1);
}
#ifdef BUF_ABORT
#undef BUF_ABORT
#endif

#define BUF_ABORT test_abort()

#if _WIN32
#  define C_RED(s)     s
#  define C_GREEN(s)   s
#else
#  define C_RED(s)     "\033[31;1m" s "\033[0m"
#  define C_GREEN(s)   "\033[32;1m" s "\033[0m"
#endif

#define TEST(s, x) \
    do { \
        if (x) { \
            printf(C_GREEN("PASS") " %s\n", s); \
            count_pass++; \
        } else { \
            printf(C_RED("FAIL") " %s\n", s); \
            count_fail++; \
        } \
    } while (0)

static uint32_t
pcg32(uint64_t *s)
{
    uint64_t m = 0x5851f42d4c957f2d;
    uint64_t a = 0x8b260b70b8e98891;
    uint64_t p = *s;
    uint32_t x = ((p >> 18) ^ p) >> 27;
    uint32_t r = p >> 59;
    *s = p * m + a;
    return (x >> r) | (x << (-r & 31u));
}

static unsigned long
bench(uint64_t *rng)
{
    unsigned long r = 0;
    uint32_t n = 1000000 + pcg32(rng) % 4000000;
    float *buf = 0;

    for (uint32_t i = 0; i < n; i++)
        buf_push(buf, pcg32(rng) / (double)UINT32_MAX);

    float threshold = pcg32(rng) / (double)UINT32_MAX;
    for (uint32_t i = 0; i < n; i++)
        r += buf[i] > threshold;

    buf_free(buf);
    return r;
}

#ifdef _WIN32
#include <windows.h>
uint64_t
uepoch(void)
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    uint64_t tt = ft.dwHighDateTime;
    tt <<= 32;
    tt |= ft.dwLowDateTime;
    tt /=10;
    tt -= UINT64_C(11644473600000000);
    return tt;
}
#else
#include <sys/time.h>
uint64_t
uepoch(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return 1000000LL * tv.tv_sec + tv.tv_usec;
}
#endif

#suite BufTests
#test init
  /* initialization, buf_free() */
  float *a = 0;
	ck_assert_int_eq(buf_capacity(a), 0);
  ck_assert_int_eq(buf_size(a), 0);
  buf_push(a, 1.3f);
  ck_assert_int_eq(buf_size(a), 1);
  ck_assert_float_eq(a[0], (float)1.3f);
  buf_clear(a);
  ck_assert_int_eq(buf_size(a), 0);
  ck_assert_ptr_nonnull(a);
  buf_free(a);
  ck_assert_ptr_null(a);
  /* Clearing an NULL pointer is a no-op */
  buf_clear(a);
  ck_assert_int_eq(buf_size(a), 0);
  ck_assert_ptr_null(a);
#test push_op
  long *ai = 0;
  for (int i = 0; i < 10000; i++)
    buf_push(ai, i);
  ck_assert_int_eq(buf_size(ai), 10000);
  int match = 0;
  for (int i = 0; i < (int)(buf_size(ai)); i++)
    match += ai[i] == i;
  ck_assert_int_eq(match, 10000);
  buf_free(ai);
#test buf_grow
  long *ai = 0;
  buf_grow(ai, 1000);
  ck_assert_int_eq(buf_capacity(ai), 1000);
  ck_assert_int_eq(buf_size(ai), 0);
  buf_trunc(ai, 100);
  ck_assert_int_eq(buf_capacity(ai), 100);
  buf_free(ai);
#test buf_pop
  long *a = 0;
  buf_push(a, 1.1);
  buf_push(a, 1.2);
  buf_push(a, 1.3);
  buf_push(a, 1.4);
  ck_assert_int_eq(buf_size(a), 4);
  ck_assert_float_eq(buf_pop(a), (float)1.4f);
  buf_trunc(a, 3);
  ck_assert_int_eq(buf_size(a), 3);
  ck_assert_float_eq(buf_pop(a), (float)1.3f);
  ck_assert_float_eq(buf_pop(a), (float)1.2f);
  ck_assert_float_eq(buf_pop(a), (float)1.1f);
  ck_assert_int_eq(buf_size(a), 0);
  buf_free(a);

