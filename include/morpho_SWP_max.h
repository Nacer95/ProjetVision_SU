/* -------------------- */
/* --- morpho_SWP_max.h --- */
/* -------------------- */

/*
 * Copyright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique
 */

#ifndef __MORPHO_SWP_MAX_H__
#define __MORPHO_SWP_MAX_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif


// line functions
// max
void line_max3_ui8matrix_swp_rotation  (uint8 **X8,   int i, int j0, int j1, uint8  **Y8);
void line_max3_ui16matrix_swp_rotation (uint16 **X16, int i, int j0, int j1, uint16 **Y16);
void line_max3_ui132matrix_swp_rotation(uint32 **X32, int i, int j0, int j1, uint32 **Y32);


// full-matrix functions
// max
void max3_ui8matrix_swp_rotation (uint8 **X, int h, int w, int i0, int i1, int j0, int j1, uint8  **Z8,  uint8 **Y);
void max3_ui16matrix_swp_rotation(uint8 **X, int h, int w, int i0, int i1, int j0, int j1, uint16 **Z16, uint8 **Y);
void max3_ui32matrix_swp_rotation(uint8 **X, int h, int w, int i0, int i1, int j0, int j1, uint32 **Z32, uint8 **Y);

void max3_ui8matrix_swp_rotation_trivial  (uint8 **X8,  int h, int w, int i0, int i1, int j0, int j1, uint8 **Z8)  ;
void max3_ui16matrix_swp_rotation_trivial(uint16 **X16, int h, int w, int i0, int i1, int j0, int j1, uint16 **Z16);
void max3_ui32matrix_swp_rotation_trivial(uint32 **X32, int h, int w, int i0, int i1, int j0, int j1, uint32 **Z32);

#ifdef __cplusplus
}
#endif

#endif // __MORPHO_MIN_H__
