/* -------------------------- */
/* --- morpho_ouverture.c --- */
/* -------------------------- */

/*
 * Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved


 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"
//#include "sequence.h"

#include "swp.h"
#include "morpho_min.h"
#include "morpho_max.h"
#include "morpho_ouverture.h"

#include "morpho_SWP_min.h"
#include "morpho_SWP_max.h"
// #include "morpho_SWP_ouverture.h"


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------            Pipeline              --------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------

void ouverture3_ui8matrix_SWP_basic(uint8 **X, int h, int w, int i0, int i1, int j0, int j1, uint8 **Z, uint8 **Y)
// ------------------------------------------------------------------------------------------------------
/*
  X la matrice
  T8 erosion compacte
  Y8 dilatation compacte
  Y ouverture
*/
{

  int r = 2;
  int w8 = j1+1;
  uint8 **T8_rotation  = ui8matrix (0-2*r, h-1+2*r, 0-2*r,  w8-1+2*r);
  uint8 **Y8_rotation  = ui8matrix (0-2*r, h-1+2*r, 0-2*r,  w8-1+2*r);

  min3_ui8matrix_swp_rotation (X, h, w, i0-1, i1+1, j0-1, j1+1, T8_rotation,  Z);
  max3_ui8matrix_swp_rotation (Z, h, w, i0, i1, j0, j1, Y8_rotation,  Y);

}

void ouverture3_ui8matrix_pipeline_SWP_rotation(uint8 **X, int h, int w, int i0, int i1, int j0, int j1, uint8 **T8, uint8 **Y8, uint8 **Z, uint8 **Y)
// ------------------------------------------------------------------------------------------------------
/*
  X la matrice
  T8 erosion compacte
  Y8 dilatation compacte
  Y ouverture
*/
{
  // pack
  int r = 2;
  int w8 = j1+1;
  uint8 **T8_rotation  = ui8matrix (0-1*r, h-1+1*r, 0-1*r,  w8-1+1*r);
  uint8 **Y8_rotation  = ui8matrix (0-1*r, h-1+1*r, 0-1*r,  w8-1+1*r);
  uint8 **X8           = ui8matrix (0-1*r, h-1+1*r, 0-1*r,  w8-1+1*r);
  pack_ui8matrix(X, h, w, X8);

  line_min3_ui8matrix_swp_rotation(X8, i0, j0, j1, T8_rotation);
  line_min3_ui8matrix_swp_rotation(X8, i0, j0, j1, T8_rotation);

  for (int i=i0; i<=i1; i++){
    line_min3_ui8matrix_swp_rotation(X8, i+1, j0, j1,  T8_rotation);
    line_max3_ui8matrix_swp_rotation(T8_rotation, i,   j0, j1,  Y8_rotation);
  }
  //unpack
  unpack_ui8matrix (T8_rotation , h, w8, Z);
  unpack_ui8matrix (Y8_rotation , h, w8, Y);



}
