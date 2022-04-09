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

#include "test_morpho_SWP.h"
#include "morpho_SWP.h"



void test_morpho_SWP_routine(int h, int w0){
  // avec des ADD
  int r = 1; // rayon des operateurs
  int w8 = w0 / 8; if(w0 % 8) w8 = w8+1;
  int w1 = 8 * w8; // w1 >= w

  puts("--------------------------------------------------");
  printf("test_morpho_SWP_routine h = %d w0 = %d w8 = %d, w1 = %d\n", h, w0, w8, w1);
  if(w1 > w0) puts("w1 > w0");

  uint8 **X;
  uint8 **X8, **X1;

  uint8 **T8_rotation, **Z8_rotation, **Y_rotation;


  char *format1; // affichage 1 bit/pixel
  char *format8; // affichage 8 bits/pixel

  //int c; // error

  format8 = "%3x";
  format1 = "%4d";
  format1 = "%2d";
  //format1 = "%1d";

  puts("malloc");
  //Allocation des matrices ====================================================
  // X 2r-border
  X      = ui8matrix(0-2*r, h-1+2*r, 0-2*r, w1-1+2*r);
  X8     = ui8matrix(0-2*r, h-1+2*r, 0-1*r, w8-1+1*r);


  // T 1r-border
  T8_rotation                        = ui8matrix(0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
  Z8_rotation                        = ui8matrix(0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);


  // Y 0r-border
  Y_rotation                         = ui8matrix(0, h-1, 0, w1-1);


  //Initialisation des matrices à zero =========================================
  //PUTS("zero X");
  zero_ui8matrix(X , 0-2*r, h-1+2*r, 0-2*r, w1-1+2*r);

  //PUTS("zero T");
  zero_ui8matrix(T8_rotation,   0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
  zero_ui8matrix(Z8_rotation,   0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);

  //PUTS("zero Y");
  zero_ui8matrix(Y_rotation      , 0-0*r, h-1+0*r, 0-0*r, w1-1+0*r);

  //faire un random pour la matrice X ==========================================
  rand1_ui8matrix(X, 0, h-1, 0, w0-1, 80);
  rand1_ui8matrix(X, 0, h-1, 0, w0-1, 90); // binaire [0,1] (afin d'avoir des '1' apres traitement et pas que des '0');
  //display_ui8matrix (X,  0-2*r, h-1+2*r, 0-2*r, w1-1+2*r, format1, "X0");

  //appel des fonctions
  swp8_ui8matrix_basic(X, w1, h, 0, h-1, 0, w8-1, w8, T8_rotation, Z8_rotation, Y_rotation );

  //display
  display_ui8matrix(Y_rotation                      ,  0, h-1, 0, w0-1, format1, "Y_rotation                    ");

  //compare
  //c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_rotation       , "Y_rotation       ");

  //free
  //free_ui8matrix(Y_rotation,                        0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
}


void test_morpho_SWP(void){
  int h0 = 8;
  int w0 = 10;

  int dh = 4;
  int dw = 3;

  dh = dw = 0;

  dh = 5; dw = 4;
  h0 = 8; w0 = 32;

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
