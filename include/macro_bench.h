/* --------------------- */
/* --- macro_bench.h --- */
/* --------------------- */

#ifndef __MACRO_BENCH_H__
#define __MACRO_BENCH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>
#include <time.h>

#define BENCH(X, n, cpp)  do {        \
  double t0, t1, dt, tmin = 1e20;     \
  int run = 4;                        \
  int iter = 2 ;                      \
  for (int r = 0; r < run; r++) {     \
     t0 = _rdtsc();                   \
     for (int k = 0; k < iter; k++) { \
        X;                            \
     }                                \
     t1 = _rdtsc();                   \
     dt = t1 - t0;                    \
     dt /= (double) iter;             \
     if (dt < tmin) {                 \
        tmin = dt;                    \
     }                                \
   }                                  \
   cpp = tmin / (n * n);              \
 } while (0)

 #define BENCH_secondes(X, n, s)  do {          \
   double  dt, tmin = 1e20;                     \
   clock_t t0 , t1;                             \
   int run = 1;                                 \
   int iter = 1 ;                               \
   for (int r = 0; r < run; r++) {              \
      t0 = clock();                             \
      for (int k = 0; k < iter; k++) {          \
         X;                                     \
      }                                         \
      t1 = clock();                             \
      dt = (double)(t1-t0)/CLOCKS_PER_SEC;      \
      dt /= (double) iter;                      \
      if (dt < tmin) {                          \
         tmin = dt;                             \
      }                                         \
    }                                           \
    s = tmin;                                   \
  } while (0)


#ifdef __cplusplus
}
#endif

#endif // __MACRO_BENCH_H__
