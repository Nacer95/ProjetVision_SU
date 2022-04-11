/* --------------------- */
/* --- morpho_test.c --- */
/* --------------------- */

/*
 * Copyright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique
 */

/* -------------- */
/* --- main.c --- */
/* -------------- */

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

#include "test_morpho_SWP.h"



void test_morpho_SWP_routine(int h, int w0){
  // avec des ADD
  int r = 1; // rayon des operateurs
  int w8 = w0 / 8; if(w0 % 8) w8 = w8+1;
  int w16 =w0 / 16; if(w0 % 16) w16 = w16+1;


  int w1_8 = 8 * w8; // w1 >= w
  int w1_16 = 16 * w16;

  //w0 la taille entrée en paramètre
  //w1 la taille de la matrice au multiple de 8 le plus proche
  //w8 la taille de la matrice compactée

  puts("--------------------------------------------------");
  printf("test_morpho_SWP_routine h = %d w0 = %d w8 = %d, w1_8 = %d\n", h, w0, w8, w1_8);
  //if(w1 > w0) puts("w1 > w0");

  uint8 **X;
  uint8 **X8, **X1;
  uint8 **Y_basic;
  uint8 **Y8_rotation, **Y8_rotation_unpack, **Y16_rotation_unpack;
  uint16 **Y16_rotation;


  char *format1; // affichage 1 bit/pixel
  char *format8; // affichage 8 bits/pixel

  //int c; // error

  format8 = "%3x";
  format1 = "%4d";
  format1 = "%2d";
  //format1 = "%1d";

  int c ;
  //puts("malloc");
  //Allocation des matrices ====================================================
  // X 2r-border
  X      = ui8matrix(0-2*r, h-1+2*r, 0-2*r, w1_16-1+2*r);//taille multiple de 8
  X8     = ui8matrix(0-2*r, h-1+2*r, 0-1*r, w1_8-1+1*r);//taille matrice compacté


  // T 1r-border
  Y8_rotation                        = ui8matrix (0-2*r, h-1+2*r, 0-1*r,  w8-1+1*r);
  Y16_rotation                       = ui16matrix(0-2*r, h-1+2*r, 0-1*r, w16-1+1*r);


  // Y 0r-border
  Y_basic                            = ui8matrix(0, h-1, 0, w1_8-1);
  Y8_rotation_unpack                 = ui8matrix(0, h-1, 0, w1_8-1);
  Y16_rotation_unpack                = ui8matrix(0, h-1, 0, w1_16-1);



  //Initialisation des matrices à zero =========================================
  //PUTS("zero X");
  zero_ui8matrix(X , 0-2*r, h-1+2*r, 0-2*r, w1_8-1+2*r);

  //PUTS("zero Y8");
  zero_ui8matrix (Y8_rotation,    0-2*r, h-1+2*r, 0-1*r, w8-1+1*r);
  zero_ui16matrix(Y16_rotation,   0-2*r, h-1+2*r, 0-1*r, w16-1+1*r);



  //PUTS("zero Y");
  zero_ui8matrix( Y8_rotation_unpack,      0-0*r, h-1+0*r, 0-0*r, w1_8-1+0*r);
  zero_ui8matrix(Y16_rotation_unpack,      0-0*r, h-1+0*r, 0-0*r, w1_16-1+0*r);


  //faire un random pour la matrice X ==========================================
  rand1_ui8matrix(X, 0, h-1, 0, w0-1, 80);
  rand1_ui8matrix(X, 0, h-1, 0, w0-1, 20); // binaire [0,1] (afin d'avoir des '1' apres traitement et pas que des '0');
  //display_ui8matrix (X,  0-2*r, h-1+2*r, 0-2*r, w1-1+2*r, format1, "X0");

  //appel des fonctions
  //pack_ui8matrix(X, h, w1, Y8_rotation);
  max3_ui8matrix_basic(X, 0, h-1, 0, w0-1, Y_basic);
  max3_ui8matrix_swp_rotation (X, h, w1_8, 0, h-1, 0, w8-1,  Y8_rotation,   Y8_rotation_unpack);
  max3_ui16matrix_swp_rotation(X, h, w1_16, 0, h-1, 0, w16-1, Y16_rotation,  Y16_rotation_unpack);


  //display
  //display_ui8matrix(X                               ,  0, h-1, 0, w0-1, format8, "X                                  ");
  display_ui8matrix(Y_basic                                 ,  0, h-1, 0, w0-1, format8, "Y_basic                            ");
  //display_ui8matrix(Y8_rotation_unpack                      ,  0, h-1, 0, w0-1, format8, "Y8_rotation_unpack                         ");
  display_ui8matrix(Y16_rotation_unpack                      ,  0, h-1, 0, w0-1, format8, "Y16_rotation_unpack                         ");



  //compare
  //c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y8_rotation_unpack       , "Y8_rotation        ");
  c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y16_rotation_unpack      , "Y16_rotation       ");


  //free
  //free_ui8matrix(Y_rotation,                        0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
}


void test_morpho_SWP(void){
  int h0 = 5;
  int w0 = 18;

  int dh = 4;
  int dw = 3;

  dh = dw = 0;

  dh = 5; dw = 4;
  // h0 = 8; w0 = 32;
  //test_morpho_SWP_routine(h0, w0);
  for(int h = h0; h <= h0+dh; h++) { // pour tester elu2
      for(int w = w0; w <= w0+dw; w++) { // pour tester ilu3
          test_morpho_SWP_routine(h, w);
      }
  }
}

int test_swp(int argc, char* argv[]){
  test_morpho_SWP();
  return 0;
}
