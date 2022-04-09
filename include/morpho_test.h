/* --------------------- */
/* --- morpho_test.h --- */
/* --------------------- */

#ifndef __MORPHO_TEST_H__
#define __MORPHO_TEST_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

int test_morpho(int argc, char* argv[]);


// function 
void rand_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, uint8 x0, uint8 x1);
void rand1_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, int density);
int compare_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, char* name);

#ifdef __cplusplus
}
#endif

#endif // __MORPHO_TEST_H__
