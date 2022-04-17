/* -------------------- */
/* --- morpho_max.c --- */
/* -------------------- */

/*
 * Copyright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"


// ------------------------------------------------------------------------
void line_max3_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------
{
  for(int j=j0; j<= j1; j++){
    store2(Y, i, j, max3(
      max3(load2(X, i-1, j-1), load2(X, i-1, j), load2(X, i-1, j+1)),
      max3(load2(X, i, j-1), load2(X, i, j), load2(X, i, j+1)),
      max3(load2(X, i+1, j-1), load2(X, i+1, j), load2(X, i+1, j+1))
      ));
  }
}
// ----------------------------------------------------------------------
void line_max3_ui8matrix_reg(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------
{
  for(int j=j0; j<= j1; j++){

    uint8 haut_gauche = load2(X, i-1, j-1);
    uint8 haut_milieu = load2(X, i-1, j);
    uint8 haut_droit  = load2(X, i-1, j+1);

    uint8 milieu_gauche = load2(X, i, j-1);
    uint8 milieu_milieu  = load2(X, i, j);
    uint8 milieu_droit   = load2(X, i, j+1);

    uint8 bas_gauche = load2(X, i+1, j-1);
    uint8 bas_milieu = load2(X, i+1, j);
    uint8 bas_droit  = load2(X, i+1, j+1);


    uint8 m1 = max3(haut_droit, haut_milieu, haut_gauche);
    uint8 m2 = max3(milieu_droit, milieu_milieu, milieu_gauche);
    uint8 m3 = max3(bas_gauche, bas_milieu, bas_droit);

    uint8 m = max3(m1, m2, m3);
    store2(Y, i, j, m);
  }
}
// ----------------------------------------------------------------------
void line_max3_ui8matrix_rot(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------
{
  uint8 haut_gauche   = load2(X, i-1, j0-1);
  uint8 milieu_gauche = load2(X, i, j0-1);
  uint8 bas_gauche    = load2(X, i+1, j0-1);

  uint8 haut_milieu   = load2(X, i-1, j0);
  uint8 milieu_milieu = load2(X, i, j0);
  uint8 bas_milieu    = load2(X, i+1, j0);


  uint8 haut_droit ;
  uint8 milieu_droit ;
  uint8 bas_droit ;

  for(int j=j0; j<= j1; j++){

    haut_droit    = load2(X, i-1, j+1);
    milieu_droit  = load2(X, i, j+1);
    bas_droit     = load2(X, i+1, j+1);

    uint8 m1 = max3(haut_droit, haut_milieu, haut_gauche);
    uint8 m2 = max3(milieu_droit, milieu_milieu, milieu_gauche);
    uint8 m3 = max3(bas_gauche, bas_milieu, bas_droit);

    uint8 m = max3(m1, m2, m3);
    store2(Y, i, j, m);

    haut_gauche = haut_milieu;
    milieu_gauche = milieu_milieu;
    bas_gauche = bas_milieu;

    haut_milieu = haut_droit;
    milieu_milieu = milieu_droit;
    bas_milieu = bas_droit;

  }
}
// ----------------------------------------------------------------------
void line_max3_ui8matrix_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------
{
  uint8 haut_gauche   = load2(X, i-1, j0-1);
  uint8 milieu_gauche = load2(X, i, j0-1);
  uint8 bas_gauche    = load2(X, i+1, j0-1);

  uint8 haut_milieu   = load2(X, i-1, j0);
  uint8 milieu_milieu = load2(X, i, j0);
  uint8 bas_milieu    = load2(X, i+1, j0);

  uint8 max_1 = max3(haut_gauche, milieu_gauche, bas_gauche);
  uint8 max_2 = max3(haut_milieu, milieu_milieu, bas_milieu);
  uint8 max_3, max;
  uint8 haut_droit, milieu_droit, bas_droit;

  for(int j=j0; j<=j1; j++){
    haut_droit    = load2(X, i-1, j+1);
    milieu_droit  = load2(X, i, j+1);
    bas_droit     = load2(X, i+1, j+1);

    max_3 = max3(haut_droit, milieu_droit, bas_droit);
    max = max3(max_1, max_2, max_3);
    store2(Y, i, j, max);

    max_1 = max_2;
    max_2 = max_3;

  }
}
// -----------------------------------------------------------------------
void line_max3_ui8matrix_ilu3(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------
{
  /*
    parcourir les cases 3 par 3
    pour une ligne traiter 3 cases puis 3 prochaine
    ce qui doit être optimisé doit être dans la boucle
    plus on avance plus c'est optimisé
  */
  /*
  uint8 j_fin;
  for (int j = j0; j<= j1; j+=3){
    //traiter j, j+1, j+2 pour le même i
    if (j+1 <= j1) { j_fin = j+1 ;}
    if (j+2 <= j1) { j_fin = j+2 ;}
    line_max3_ui8matrix_basic(X, i, j, j_fin, Y);
  }
  */
  uint8 haut_gauche   = load2(X, i-1, j0-1);
  uint8 milieu_gauche = load2(X, i, j0-1);
  uint8 bas_gauche    = load2(X, i+1, j0-1);

  uint8 haut_milieu   = load2(X, i-1, j0);
  uint8 milieu_milieu = load2(X, i, j0);
  uint8 bas_milieu    = load2(X, i+1, j0);

  uint8 haut_droit ;
  uint8 milieu_droit ;
  uint8 bas_droit ;

  int j_tmp;
  int j;
  int reste = (j1 - j0) % 3;

  for (j=j0; j<=(j1-reste); j+=3 ){
    // J
    haut_droit    = load2(X, i-1, j+1);
    milieu_droit  = load2(X, i, j+1);
    bas_droit     = load2(X, i+1, j+1);

    uint8 m1 = max3(haut_droit, haut_milieu, haut_gauche);
    uint8 m2 = max3(milieu_droit, milieu_milieu, milieu_gauche);
    uint8 m3 = max3(bas_gauche, bas_milieu, bas_droit);

    uint8 m = max3(m1, m2, m3);
    store2(Y, i, j, m);

    // J+1
    j_tmp = j+1;

    haut_gauche   = load2(X, i-1, j_tmp+1);
    milieu_gauche = load2(X, i, j_tmp+1);
    bas_gauche    = load2(X, i+1, j_tmp+1);


    m1 = max3(haut_droit, haut_milieu, haut_gauche);
    m2 = max3(milieu_droit, milieu_milieu, milieu_gauche);
    m3 = max3(bas_gauche, bas_milieu, bas_droit);

    m = max3(m1, m2, m3);
    store2(Y, i, j_tmp, m);

    // J+2
    j_tmp = j+2;

    haut_milieu   = load2(X, i-1, j_tmp+1);
    milieu_milieu = load2(X, i, j_tmp+1);
    bas_milieu    = load2(X, i+1, j_tmp+1);

    m1 = max3(haut_droit, haut_milieu, haut_gauche);
    m2 = max3(milieu_droit, milieu_milieu, milieu_gauche);
    m3 = max3(bas_gauche, bas_milieu, bas_droit);

    m = max3(m1, m2, m3);
    store2(Y, i, j_tmp, m);

  }
  for (int r = j1-reste+1 ; r<= j1; r++){
    line_max3_ui8matrix_basic(X, i, j1-r+1, j1, Y);
  }
}
// ---------------------------------------------------------------------------
void line_max3_ui8matrix_ilu3_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------
{
  uint8 haut_gauche   = load2(X, i-1, j0-1);
  uint8 milieu_gauche = load2(X, i, j0-1);
  uint8 bas_gauche    = load2(X, i+1, j0-1);

  uint8 haut_milieu   = load2(X, i-1, j0);
  uint8 milieu_milieu = load2(X, i, j0);
  uint8 bas_milieu    = load2(X, i+1, j0);

  uint8 m_milieu = max3(haut_milieu, milieu_milieu, bas_milieu);
  uint8 m_gauche = max3(haut_gauche, milieu_gauche, bas_gauche);

  uint8 haut_droit ;
  uint8 milieu_droit ;
  uint8 bas_droit ;

  int j_tmp;
  int j;
  int reste = (j1 - j0) % 3;

  for (j=j0; j<= (j1-reste); j+=3 ){
    // J
    haut_droit    = load2(X, i-1, j+1);
    milieu_droit  = load2(X, i, j+1);
    bas_droit     = load2(X, i+1, j+1);

    uint8 m_droit = max3(haut_droit, milieu_droit, bas_droit);
    uint8 m = max3(m_gauche, m_milieu, m_droit);
    store2(Y, i, j, m);

    // J+1
    j_tmp = j+1;

    haut_gauche   = load2(X, i-1, j_tmp+1);
    milieu_gauche = load2(X, i, j_tmp+1);
    bas_gauche    = load2(X, i+1, j_tmp+1);

    m_gauche = max3(haut_gauche, milieu_gauche, bas_gauche);

    m = max3(m_gauche, m_milieu, m_droit);
    store2(Y, i, j_tmp, m);

    // J+2
    j_tmp = j+2;

    haut_milieu   = load2(X, i-1, j_tmp+1);
    milieu_milieu = load2(X, i, j_tmp+1);
    bas_milieu    = load2(X, i+1, j_tmp+1);

    m_milieu = max3(haut_milieu, milieu_milieu, bas_milieu);

    m = max3(m_gauche, m_milieu, m_droit);
    store2(Y, i, j_tmp, m);

  }
  //printf("j= %d, j1=%d\n", j, j1);
  // for (int r = j1-reste+1 ; r<= j1; r++){
  if (reste){
    line_max3_ui8matrix_red(X, i, j1-reste+1, j1, Y);
  }
}
// ---------------------------------------------------------------------------
void line_max3_ui8matrix_elu2_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------
{
  uint8 haut_gauche   = load2(X, i-1, j0-1);
  uint8 milieu_gauche = load2(X, i, j0-1);
  uint8 bas_gauche    = load2(X, i+1, j0-1);
  uint8 bas2_gauche   = load2(X, i+2, j0-1);

  uint8 haut_milieu   = load2(X, i-1, j0);
  uint8 milieu_milieu = load2(X, i, j0);
  uint8 bas_milieu    = load2(X, i+1, j0);
  uint8 bas2_milieu   = load2(X, i+2, j0);

  uint8 max_i1_gauche, max_i1_milieu, max_i1_droit, max_i1;
  uint8 max_i2_gauche, max_i2_milieu, max_i2_droit, max_i2;

  max_i1_gauche = max3(haut_gauche, milieu_gauche, bas_gauche);
  max_i1_milieu = max3(haut_milieu, milieu_milieu, bas_milieu);

  max_i2_gauche = max3(milieu_gauche, bas_gauche, bas2_gauche);
  max_i2_milieu = max3(milieu_milieu, bas_milieu, bas2_milieu);

  uint8 haut_droit, milieu_droit, bas_droit, bas2_droit;
  int j, i_tmp, reste;


  for(j=j0; j<=j1; j++){
    //printf("j=%d  j1 = %d , i=%d\n", j, j1, i);

    haut_droit    = load2(X, i-1, j+1);
    milieu_droit  = load2(X, i, j+1);
    bas_droit     = load2(X, i+1, j+1);
    bas2_droit    = load2(X, i+2, j+1);

    //--------------- ligne 1 -----------

    max_i1_droit  = max3(haut_droit, milieu_droit, bas_droit);
    max_i1 = max3(max_i1_gauche, max_i1_milieu, max_i1_droit);
    store2(Y, i, j, max_i1);

    //--------------- ligne 2 ------------
    i_tmp = i+1;

    max_i2_droit  = max3(milieu_droit, bas_droit, bas2_droit);
    max_i2 = max3(max_i2_gauche, max_i2_milieu, max_i2_droit);
    store2(Y, i_tmp, j, max_i2);


    //--------------- ligne 1 -----------
    max_i1_gauche = max_i1_milieu;
    max_i1_milieu = max_i1_droit;

    //--------------- ligne 2 ------------
    max_i2_gauche = max_i2_milieu;
    max_i2_milieu = max_i2_droit;
  }
}
// ----------------------------------------------------------------------------------
void line_max3_ui8matrix_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------
{
  uint8 haut_gauche   = load2(X, i-1, j0-1);
  uint8 milieu_gauche = load2(X, i, j0-1);
  uint8 bas_gauche    = load2(X, i+1, j0-1);
  uint8 bas2_gauche   = load2(X, i+2, j0-1);

  uint8 haut_milieu   = load2(X, i-1, j0);
  uint8 milieu_milieu = load2(X, i, j0);
  uint8 bas_milieu    = load2(X, i+1, j0);
  uint8 bas2_milieu   = load2(X, i+2, j0);

  uint8 max_i1_gauche, max_i1_milieu, max_i1_droit, max_i1;
  uint8 max_i2_gauche, max_i2_milieu, max_i2_droit, max_i2;

  uint8 max_milieu_gauche_bas_gauche = max2(milieu_gauche, bas_gauche);
  uint8 max_milieu_milieu_bas_milieu = max2(milieu_milieu, bas_milieu);

  max_i1_gauche = max2(haut_gauche, max_milieu_gauche_bas_gauche);
  max_i1_milieu = max2(haut_milieu, max_milieu_milieu_bas_milieu);

  max_i2_gauche = max2(max_milieu_gauche_bas_gauche, bas2_gauche);
  max_i2_milieu = max2(max_milieu_milieu_bas_milieu, bas2_milieu);

  uint8 haut_droit, milieu_droit, bas_droit, bas2_droit;
  int j, i_tmp, reste;

  for(j=j0; j<=j1; j++){
    //printf("j=%d  j1 = %d , i=%d\n", j, j1, i);

    haut_droit    = load2(X, i-1, j+1);
    milieu_droit  = load2(X, i, j+1);
    bas_droit     = load2(X, i+1, j+1);
    bas2_droit    = load2(X, i+2, j+1);

    //--------------- commun ------------
    uint8 max_milieu_droit_bas_droit = max2(milieu_droit, bas_droit);

    //--------------- ligne 1 -----------

    max_i1_droit  = max2(haut_droit, max_milieu_droit_bas_droit);
    max_i1 = max3(max_i1_gauche, max_i1_milieu, max_i1_droit);
    store2(Y, i, j, max_i1);

    //--------------- ligne 2 ------------
    i_tmp = i+1;
    max_i2_droit  = max2(max_milieu_droit_bas_droit, bas2_droit);

    max_i2 = max3(max_i2_gauche, max_i2_milieu, max_i2_droit);
    store2(Y, i_tmp, j, max_i2);


    //--------------- ligne 1 -----------
    max_i1_gauche = max_i1_milieu;
    max_i1_milieu = max_i1_droit;

    //--------------- ligne 2 ------------
    max_i2_gauche = max_i2_milieu;
    max_i2_milieu = max_i2_droit;
  }
}
// --------------------------------------------------------------------------------
void line_max3_ui8matrix_ilu3_elu2_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------
{
  uint8 haut_gauche   = load2(X, i-1, j0-1);//
  uint8 milieu_gauche = load2(X, i, j0-1);//
  uint8 bas_gauche    = load2(X, i+1, j0-1);//
  uint8 bas2_gauche   = load2(X, i+2, j0-1);

  uint8 haut_milieu   = load2(X, i-1, j0);
  uint8 milieu_milieu = load2(X, i, j0);//
  uint8 bas_milieu    = load2(X, i+1, j0);//
  uint8 bas2_milieu   = load2(X, i+2, j0);

  uint8 max_i1_gauche, max_i1_milieu, max_i1_droit, max_i1;
  uint8 max_i2_gauche, max_i2_milieu, max_i2_droit, max_i2;

  // ---------------------- commun -----------------------
  uint8 max_milieu_gauche_bas_gauche = max2(milieu_gauche, bas_gauche);
  uint8 max_milieu_milieu_bas_milieu = max2(milieu_milieu, bas_milieu);

  max_i1_gauche = max2(haut_gauche, max_milieu_gauche_bas_gauche);
  max_i1_milieu = max2(haut_milieu, max_milieu_milieu_bas_milieu);

  max_i2_gauche = max2(max_milieu_gauche_bas_gauche, bas2_gauche);
  max_i2_milieu = max2(max_milieu_milieu_bas_milieu, bas2_milieu);

  int reste = (j1 - j0 + 1) % 3; //de 0 à 2
  int j, j_temp;
  int i_tmp;
  int eluX = 2;
  for(j = j0; j <= (j1-reste); j+=3){
    //3e colonne
    uint8 haut_droit    = load2(X, i-1, j+1);
    uint8 milieu_droit  = load2(X, i, j+1);
    uint8 bas_droit     = load2(X, i+1, j+1);
    uint8 bas2_droit    = load2(X, i+2, j+1);
    //J


    //--------------- ligne 1 -----------
    max_i1_droit  = max3(haut_droit, milieu_droit, bas_droit);
    max_i1 = max3(max_i1_gauche, max_i1_milieu, max_i1_droit);
    store2(Y, i, j, max_i1);
    //--------------- ligne 2 ------------
    i_tmp = i+1;
    max_i2_droit  = max3(milieu_droit, bas_droit, bas2_droit);
    max_i2 = max3(max_i2_gauche, max_i2_milieu, max_i2_droit);
    store2(Y, i_tmp, j, max_i2);

    //J+1 ===================================================
    j_temp = j+1;
    haut_gauche   = load2(X, i-1, j+2);
    milieu_gauche = load2(X, i,   j+2);
    bas_gauche    = load2(X, i+1, j+2);
    bas2_gauche   = load2(X, i+2, j+2);

    //--------------- ligne 1 -----------

    max_i1_gauche = max3(haut_gauche, milieu_gauche, bas_gauche);
    max_i1 = max3(max_i1_gauche, max_i1_milieu, max_i1_droit);
    store2(Y, i, j_temp, max_i1);
    //--------------- ligne 2 ------------
    i_tmp = i+1;

    max_i2_gauche = max3(milieu_gauche, bas_gauche, bas2_gauche);
    max_i2 = max3(max_i2_gauche, max_i2_milieu, max_i2_droit);
    store2(Y, i_tmp, j_temp, max_i2);

    //J+2 ==========================================
    j_temp = j+2;
    haut_milieu   = load2(X, i-1, j_temp+1);
    milieu_milieu = load2(X, i, j_temp+1);
    bas_milieu    = load2(X, i+1, j_temp+1);
    bas2_milieu   = load2(X, i+2, j_temp+1);

    //--------------- ligne 1 -----------

    max_i1_milieu = max3(haut_milieu, milieu_milieu, bas_milieu);
    max_i1 = max3(max_i1_gauche, max_i1_milieu, max_i1_droit);
    store2(Y, i, j_temp, max_i1);
    //--------------- ligne 2 ------------
    i_tmp = i+1;

    max_i2_milieu = max3(milieu_milieu, bas_milieu, bas2_milieu);
    max_i2 = max3(max_i2_gauche, max_i2_milieu, max_i2_droit);
    store2(Y, i_tmp, j_temp, max_i2);

  }

  // for (int r=j1-reste+1; r<=j1; r++){
  if (reste){
    for (int l= 0; l<eluX; l++){
      line_max3_ui8matrix_red(X, i+l, j1-reste+1, j1, Y);
    }
  }
}
// ---------------------------------------------------------------------------------------
void line_max3_ui8matrix_ilu3_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------
{
  uint8 haut_gauche   = load2(X, i-1, j0-1);//
  uint8 milieu_gauche = load2(X, i, j0-1);//
  uint8 bas_gauche    = load2(X, i+1, j0-1);//
  uint8 bas2_gauche   = load2(X, i+2, j0-1);

  uint8 haut_milieu   = load2(X, i-1, j0);
  uint8 milieu_milieu = load2(X, i, j0);//
  uint8 bas_milieu    = load2(X, i+1, j0);//
  uint8 bas2_milieu   = load2(X, i+2, j0);

  uint8 max_i1_gauche, max_i1_milieu, max_i1_droit, max_i1;
  uint8 max_i2_gauche, max_i2_milieu, max_i2_droit, max_i2;

  // ---------------------- commun -----------------------
  uint8 max_milieu_gauche_bas_gauche = max2(milieu_gauche, bas_gauche);
  uint8 max_milieu_milieu_bas_milieu = max2(milieu_milieu, bas_milieu);

  max_i1_gauche = max2(haut_gauche, max_milieu_gauche_bas_gauche);
  max_i1_milieu = max2(haut_milieu, max_milieu_milieu_bas_milieu);

  max_i2_gauche = max2(max_milieu_gauche_bas_gauche, bas2_gauche);
  max_i2_milieu = max2(max_milieu_milieu_bas_milieu, bas2_milieu);

  int reste = (j1 - j0 + 1) % 3; //de 0 à 2
  int j, j_temp;
  int i_tmp;
  int eluX = 2;
  for(j = j0; j <= (j1-reste); j+=3){
    //3e colonne
    uint8 haut_droit    = load2(X, i-1, j+1);
    uint8 milieu_droit  = load2(X, i, j+1);
    uint8 bas_droit     = load2(X, i+1, j+1);
    uint8 bas2_droit    = load2(X, i+2, j+1);
    //J

    //--------------- commun  -----------
    uint8 max_milieu_droit_bas_droit = max2(milieu_droit, bas_droit);
    //--------------- ligne 1 -----------
    max_i1_droit  = max2(haut_droit, max_milieu_droit_bas_droit);
    max_i1 = max3(max_i1_gauche, max_i1_milieu, max_i1_droit);
    store2(Y, i, j, max_i1);
    //--------------- ligne 2 ------------
    i_tmp = i+1;
    max_i2_droit  = max3(milieu_droit, bas_droit, bas2_droit);
    max_i2 = max3(max_i2_gauche, max_i2_milieu, max_i2_droit);
    store2(Y, i_tmp, j, max_i2);

    //J+1 ===================================================
    j_temp = j+1;
    haut_gauche   = load2(X, i-1, j+2);
    milieu_gauche = load2(X, i,   j+2);
    bas_gauche    = load2(X, i+1, j+2);
    bas2_gauche   = load2(X, i+2, j+2);

    //--------------- commun  -----------
    uint8 max_milieu_gauche_bas_gauche = max2(milieu_gauche, bas_gauche);

    //--------------- ligne 1 -----------

    max_i1_gauche = max2(haut_gauche, max_milieu_gauche_bas_gauche);
    max_i1 = max3(max_i1_gauche, max_i1_milieu, max_i1_droit);
    store2(Y, i, j_temp, max_i1);
    //--------------- ligne 2 ------------
    i_tmp = i+1;

    max_i2_gauche = max2(max_milieu_gauche_bas_gauche, bas2_gauche);
    max_i2 = max3(max_i2_gauche, max_i2_milieu, max_i2_droit);
    store2(Y, i_tmp, j_temp, max_i2);

    //J+2 ==========================================
    j_temp = j+2;
    haut_milieu   = load2(X, i-1, j_temp+1);
    milieu_milieu = load2(X, i, j_temp+1);
    bas_milieu    = load2(X, i+1, j_temp+1);
    bas2_milieu   = load2(X, i+2, j_temp+1);
    //--------------- commun  -----------
    uint8 max_milieu_milieu_bas_milieu = max2(milieu_milieu, bas_milieu);

    //--------------- ligne 1 -----------

    max_i1_milieu = max2(haut_milieu, max_milieu_milieu_bas_milieu);
    max_i1 = max3(max_i1_gauche, max_i1_milieu, max_i1_droit);
    store2(Y, i, j_temp, max_i1);
    //--------------- ligne 2 ------------
    i_tmp = i+1;

    max_i2_milieu = max2(max_milieu_milieu_bas_milieu, bas2_milieu);
    max_i2 = max3(max_i2_gauche, max_i2_milieu, max_i2_droit);
    store2(Y, i_tmp, j_temp, max_i2);

  }

  // for (int r=j1-reste+1; r<=j1; r++){
  if (reste){
    for (int l= 0; l<eluX; l++){
      line_max3_ui8matrix_red(X, i+l, j1-reste+1, j1, Y);
    }
  }
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ---------------------------    Matrices    ---------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void max3_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------
{
  for (int i = i0; i<= i1; i++){
    line_max3_ui8matrix_basic(X, i, j0, j1, Y);
  }
}
// --------------------------------------------------------------------------
void max3_ui8matrix_reg(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------
{
  for (int i = i0; i<= i1; i++){
    line_max3_ui8matrix_reg(X, i, j0, j1, Y);
  }
}
// --------------------------------------------------------------------------
void max3_ui8matrix_rot(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------
{
  for (int i = i0; i<= i1; i++){
    line_max3_ui8matrix_rot(X, i, j0, j1, Y);
  }
}
// --------------------------------------------------------------------------
void max3_ui8matrix_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------
{
  for (int i= i0; i<=i1; i++){
    line_max3_ui8matrix_red(X, i, j0, j1, Y);
  }
}
// ---------------------------------------------------------------------------
void max3_ui8matrix_ilu3(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------
{
  for (int i = i0; i<= i1; i++){
    line_max3_ui8matrix_ilu3(X, i, j0, j1, Y);
  }
}
// -------------------------------------------------------------------------------
void max3_ui8matrix_ilu3_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------
{
  for (int i = i0; i<= i1; i++){
    line_max3_ui8matrix_ilu3_red(X, i, j0, j1, Y);
  }
}
// -------------------------------------------------------------------------------
void max3_ui8matrix_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------
{
  int reste = (i1-i0+1)%2;
  int i;

  for ( i= i0; i<= (i1-reste) ; i+=2){
    line_max3_ui8matrix_elu2_red(X, i, j0, j1, Y);
  }

  for (int r= i1-reste + 1; r <= i1; r++){ // Je garde cette boucle pour avoir une api générique
    line_max3_ui8matrix_red(X, i, j0, j1, Y );
  }
}
// --------------------------------------------------------------------------------------
void max3_ui8matrix_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------
{
  int reste = (i1-i0+1)%2;
  int i;

  for ( i= i0; i<= (i1-reste) ; i+=2){
    line_max3_ui8matrix_elu2_red(X, i, j0, j1, Y);
  }
  for (int r= i1-reste + 1; r <= i1; r++){ // Je garde cette boucle pour avoir une api générique
    line_max3_ui8matrix_red(X, i, j0, j1, Y );
  }
}
// ------------------------------------------------------------------------------------
void max3_ui8matrix_ilu3_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
  int eluX = 2;
  int reste = (i1-i0+1)%eluX;
  int i;

  for ( i= i0; i<= (i1-reste) ; i+=2){
    line_max3_ui8matrix_ilu3_elu2_red(X, i, j0, j1, Y);
  }
  for (int r=i1-reste+1; r <= i1; r++){ // Je garde cette boucle pour avoir une api générique
    line_max3_ui8matrix_red(X, r, j0, j1, Y );
  }
}
// -------------------------------------------------------------------------------------------
void max3_ui8matrix_ilu3_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------------
{
  int eluX = 2;
  int reste = (i1-i0+1)%eluX;
  int i;

  for ( i= i0; i<= (i1-reste) ; i+=2){
    line_max3_ui8matrix_ilu3_elu2_red(X, i, j0, j1, Y);
  }
  for (int r=i1-reste+1; r <= i1; r++){ // Je garde cette boucle pour avoir une api générique
    line_max3_ui8matrix_red(X, r, j0, j1, Y );
  }
}
