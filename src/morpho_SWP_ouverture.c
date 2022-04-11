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

void ouverture3_ui8matrix_pipeline_SWP_rotation(uint8 **X, int h, int w, int i0, int i1, int j0, int j1, uint8 **T8, uint8 **Y8, uint8 **Y)
// ------------------------------------------------------------------------------------------------------
{
  //pack
  // int r       = 2;
  // int w8      = j1+1;
  // uint8 **X8  = ui8matrix(0-1*r, h-1+1*r, 0-1*r, w8-1+1*r);
  // pack_ui8matrix(X, h, w, X8);
  //
  // line_min3_ui8matrix_swp_rotation(X8, i0-1, j0-1, j1+1, T8);
  // line_min3_ui8matrix_swp_rotation(X8, i0,   j0-1, j1+1, T8);
  //
  // for (int i=i0; i<=i1; i++){
  //   line_min3_ui8matrix_swp_rotation(X8, i+1, j0-1, j1+1,  T8);
  //   line_max3_ui8matrix_swp_rotation(T8, i, j0, j1, Y8);
  // }
  // //unpack
  // unpack_ui8matrix (Y8 , h, w8, Y);
  uint8 **Y_tmp                     = ui8matrix(0, h-1, 0, w-1);
  min3_ui8matrix_swp_rotation (X, h, w, i0, i1, j0, j1, Y8,  Y);

  // max3_ui8matrix_swp_rotation (Y_tmp, h, w, i0, i1, j0, j1, Y8,  Y);

}
