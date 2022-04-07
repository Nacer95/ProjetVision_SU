/* ---------------------- */
/* --- swp_function.c --- */
/* ---------------------- */

/*
 * Copyright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stddef.h>
 #include <string.h>
 #include <math.h>

 #include "nrtype.h"
 #include "nrdef.h"
 #include "nrutil.h"

 #include "macro_bench.h"
 #include "x86intrin.h" // _rdtsc()

 #include "swp.h"
 #include "morpho_max.h"
 #include "morpho_min.h"
 #include "morpho_ouverture.h"

 #include "morpho_test.h"
// ------------------------------------------------------------------------
void line_swp8_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------
{
  // charger 9 mots

  // faire un or = min  sur les colonnes

  // faire un min sur les 3 mots restant 
}
// ------------------------------------------------------------------------
void line_swp16_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------
{
}
// ------------------------------------------------------------------------
void line_swp32_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------
{
}

void line_swp64_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------
{
}
// ----------------------------------------------------------------------------
void swp8_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------
{
}
void swp16_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------
{
}
void swp32_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------
{
}
void swp64_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------
{
}
