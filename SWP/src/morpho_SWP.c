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
 #include "morpho_SWP.h"



void testing(){
  printf("réussit\n");
}

// ------------------------------------------------------------------------
void line_swp8_ui8matrix_basic(uint8 **T, int i, int j0, int j1, uint8 **Z)
// ------------------------------------------------------------------------
{
  //charger la bordure de gauche
  uint8 haut_gauche = 0;
  uint8 milieu_gauche = 0;
  uint8 bas_gauche = 0;

  uint8 haut_milieu   = load2(T, i-1 , j0 );
  uint8 milieu_milieu = load2(T, i   , j0 );
  uint8 bas_milieu    = load2(T, i+1 , j0 );

  uint8 min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
  uint8 min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);

  uint8 haut_droit,  milieu_droit,  bas_droit, min3_droite;
  uint8 mot_pemutation_gauche , mot_permutation_droit;
  uint8 min3_res;



  // pas d'epilogue car utilisation du padding
  for (int j = j0; j<= j1; j++){

    haut_droit    = load2(T, i-1,   j+1);
    milieu_droit  = load2(T, i  ,   j+1);
    bas_droit     = load2(T, i+1,   j+1);

    min3_droite  = mot_min3(haut_droit,  milieu_droit,  bas_droit);

    //permutation
    mot_pemutation_gauche = macro_ui8right1(min3_gauche, min3_milieu);
    mot_permutation_droit = macro_ui8left1(min3_droite, min3_milieu);

    //min
    min3_res = min3(mot_pemutation_gauche, min3_milieu, mot_permutation_droit);

    //store
    store2(Z, i, j, min3_res);


    //permutation colonne gauche
    haut_gauche   = haut_milieu;
    milieu_gauche = milieu_milieu;
    bas_gauche    = bas_milieu;

    haut_milieu   = haut_droit;
    milieu_milieu = milieu_droit;
    bas_milieu    = bas_droit;

  }
}
// ------------------------------------------------------------------------
// void line_swp16_ui8matrix_basic(uint8 *X, int i, int j0, int j1, uint16 *T, uint8 *Y)
// // ------------------------------------------------------------------------
// {
// }
// // ------------------------------------------------------------------------
// void line_swp32_ui8matrix_basic(uint8 *X, int i, int j0, int j1, uint32 *T, uint8 *Y)
// // ------------------------------------------------------------------------
// {
// }
// void line_swp64_ui8matrix_basic(uint8 *X, int i, int j0, int j1, uint64 *T, uint8 *Y)
// // ------------------------------------------------------------------------
// {
// }
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


void swp8_ui8matrix_basic(uint8 **X, int wX, int hX,  int i0, int i1, int j0, int j1,int w8, uint8 **T8, uint8 **Z8, uint8 **Y)
// ----------------------------------------------------------------------------
{
  printf("s\n");
  // faire le pack de X dans T
  pack_ui8matrix(X, hX, wX, T8);
  // calcule de chaque ligne
  for(int i = i0; i<=i1; i++){

    line_swp8_ui8matrix_basic(T8, i, j0, j1, Z8);
  }
  // unpack z dans Y
  unpack_ui8matrix (Z8 , hX, w8, Y);

}
// void swp16_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint16 **T, uint8 **Y)
// // ----------------------------------------------------------------------------
// {
// }
// void swp32_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint32 **T, uint8 **Y)
// // ----------------------------------------------------------------------------
// {
// }
// void swp64_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint64 **T, uint8 **Y)
// // ----------------------------------------------------------------------------
// {
// }
