/* -------------------- */
/* --- morpho_SWP.h --- */
/* -------------------- */

/*
 * Copyright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique
 */

#ifndef __MORPHO_SWP_H__
#define __MORPHO_SWP_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

// line functions
void line_swp8_ui8matrix_basic(uint8 **T, int i, int j0, int j1, uint8 **Z);
// void line_swp16_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint16 **T, uint8 **Y);
// void line_swp32_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint32 **T, uint8 **Y);
// void line_swp64_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint64 **T, uint8 **Y);

// full-matrix functions
void testing();
void swp8_ui8matrix_basic(uint8 **X, int wX, int hX,  int i0, int i1, int j0, int j1, int w8, uint8 **T, uint8 **Z, uint8 **Y);
// void swp16_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint16 **T, uint8 **Y);
// void swp32_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint32 **T, uint8 **Y);
// void swp64_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint64 **T, uint8 **Y);



#ifdef __cplusplus
}
#endif

#endif // __MORPHO_MIN_H__
