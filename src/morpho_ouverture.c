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

// -------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------
{

}
// ----------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_ilu5_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------------
{

}
// ---------------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_ilu5_elu2_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------------
{

}
// ----------------------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_ilu5_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------------------------
{
}
// -----------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_ilu15_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
}
// ---------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z)
// ---------------------------------------------------------------------------------------------
{
    min3_ui8matrix_basic(X, i0-1, i1+1, j0-1, j1+1, Y);
    max3_ui8matrix_basic(Y, i0,   i1,   j0,   j1,   Z);
}
// -----------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------
{
}
// --------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_ilu5_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------
{
}
// -------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_ilu5_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------------------
{
}
// --------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_ilu5_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------------------
{
}
// ---------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_ilu15_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------------
{
}
// ------------------------------------------------------------------------------------------------------
/*
  D'après ouverture3_ui8matrix_basic on suppose que la matrice X possède une bordure de 2
*/
/*
  Les opérateurs sur les images sont d ́ecompos ́es en op ́erateurs “ligne”.
  Ce sont ces opérateurs “ligne” qui sont pipelin ́es. Cela permet d’am ́eliorer la persistance
  des données dans les caches proches du processeurs. Cette opti- misation est particulièrement
  efficace dans un contexte multithread ́e. On parle alors de Cache Level Parallelism.
*/
/*
  Une ouverture est une la dilatation d'une érosion, c'est un min suivit d'un max
  Sachant que l'érosion comme la dilatation d'une pixel est déduite de son voisinage.
  Il faut que le voisinage d'un pixel est déjà subit une érosion avant qu'il subissent une dilatation
*/
void ouverture3_ui8matrix_pipeline_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ------------------------------------------------------------------------------------------------------
{
  line_min3_ui8matrix_basic(X, i0-1, j0-1, j1+1, T);
  line_min3_ui8matrix_basic(X, i0,   j0-1, j1+1, T);

  for (int i=i0; i<=i1; i++){
    line_min3_ui8matrix_basic(X, i+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_basic(T, i, j0, j1, Y);
  }

}
// ----------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ----------------------------------------------------------------------------------------------------
{
  line_min3_ui8matrix_red(X, i0-1, j0-1, j1+1, T);
  line_min3_ui8matrix_red(X, i0,   j0-1, j1+1, T);

  for (int i=i0; i<=i1; i++){
    line_min3_ui8matrix_red(X, i+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_red(T, i, j0, j1, Y);
  }

}
// ---------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_ilu3_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ---------------------------------------------------------------------------------------------------------
{
  line_min3_ui8matrix_ilu3_red(X, i0-1, j0-1, j1+1, T);
  line_min3_ui8matrix_ilu3_red(X, i0,   j0-1, j1+1, T);

  for (int i=i0; i<=i1; i++){
    line_min3_ui8matrix_ilu3_red(X, i+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_ilu3_red(T, i, j0, j1, Y);
  }
}
// ---------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ---------------------------------------------------------------------------------------------------------
{
  line_min3_ui8matrix_elu2_red(X, i0-1, j0-1, j1+1,  T);
  int eluX = 2;
  int reste = (i1 - i0 + 1 ) % eluX;

  for (int i=i0; i<=(i1-reste); i+=2){
    line_min3_ui8matrix_elu2_red(X, i+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_elu2_red(T, i, j0, j1, Y);
  }
  for (int r= i1-reste+1; r<=i1; r++){
    line_min3_ui8matrix_ilu3_red(X, r+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_ilu3_red(T, r, j0, j1, Y);
  }

}
// ----------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ----------------------------------------------------------------------------------------------------------------
{
  line_min3_ui8matrix_elu2_red_factor(X, i0-1, j0-1, j1+1,  T);
  int eluX = 2;
  int reste = (i1 - i0 + 1 ) % eluX;

  for (int i=i0; i<=(i1-reste); i+=2){
    line_min3_ui8matrix_elu2_red_factor(X, i+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_elu2_red_factor(T, i, j0, j1, Y);
  }
  for (int r= i1-reste+1; r<=i1; r++){
    line_min3_ui8matrix_ilu3_red(X, r+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_ilu3_red(T, r, j0, j1, Y);
  }

}
// --------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_ilu3_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// --------------------------------------------------------------------------------------------------------------
{
  line_min3_ui8matrix_ilu3_elu2_red(X, i0-1, j0-1, j1+1,  T);
  int eluX = 2;
  int reste = (i1 - i0 + 1 ) % eluX;

  for (int i=i0; i<=(i1-reste); i+=2){
    line_min3_ui8matrix_ilu3_elu2_red(X, i+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_ilu3_elu2_red(T, i, j0, j1, Y);
  }
  for (int r= i1-reste+1; r<=i1; r++){
    line_min3_ui8matrix_ilu3_red(X, r+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_ilu3_red(T, r, j0, j1, Y);
  }
}
// ---------------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_ilu3_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ---------------------------------------------------------------------------------------------------------------------
{
  line_min3_ui8matrix_ilu3_elu2_red_factor(X, i0-1, j0-1, j1+1,  T);
  int eluX = 2;
  int reste = (i1 - i0 + 1 ) % eluX;

  for (int i=i0; i<=(i1-reste); i+=2){
    line_min3_ui8matrix_ilu3_elu2_red_factor(X, i+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_ilu3_elu2_red_factor(T, i, j0, j1, Y);
  }
  for (int r= i1-reste+1; r<=i1; r++){
    line_min3_ui8matrix_ilu3_red(X, r+1, j0-1, j1+1,  T);
    line_max3_ui8matrix_ilu3_red(T, r, j0, j1, Y);
  }
}
