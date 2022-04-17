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

 #include "nrtype.h"
 #include "nrdef.h"
 #include "nrutil.h"
 #include "swp.h"

 // ------------------------------------------------------------------------
 void line_min3_ui8matrix_swp_rotation(uint8 **X8, int i, int j0, int j1, uint8 **Y8)
 // ------------------------------------------------------------------------
 {
     //charger la bordure de gauche
     uint8 haut_gauche = 0;
     uint8 milieu_gauche = 0;
     uint8 bas_gauche = 0;

     uint8 haut_milieu   = load2(X8, i-1 , j0);
     uint8 milieu_milieu = load2(X8, i   , j0);
     uint8 bas_milieu    = load2(X8, i+1 , j0);

     uint8 min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
     uint8 min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);

     uint8 haut_droit,  milieu_droit,  bas_droit, min3_droite;
     uint8 mot_pemutation_gauche , mot_permutation_droit;
     uint8 min3_res;
     // pas d'epilogue car utilisation du padding
     for (int j = j0; j<= j1; j++){
         haut_droit    = load2(X8, i-1,   j+1);
         milieu_droit  = load2(X8, i  ,   j+1);
         bas_droit     = load2(X8, i+1,   j+1);

         min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
         min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);
         min3_droite  = mot_min3(haut_droit,  milieu_droit,  bas_droit);

         //permutation
         mot_pemutation_gauche = macro_ui8left1(min3_gauche, min3_milieu);
         mot_permutation_droit = macro_ui8right1(min3_milieu, min3_droite);
         //min
         min3_res = mot_min3(mot_pemutation_gauche, min3_milieu, mot_permutation_droit);
         // printf("j : %d, r : %d\n", j , min3_res);
         //store
         store2(Y8, i, j, min3_res);
         //permutation colonne gauche
         haut_gauche   = haut_milieu;
         milieu_gauche = milieu_milieu;
         bas_gauche    = bas_milieu;

         haut_milieu   = haut_droit;
         milieu_milieu = milieu_droit;
         bas_milieu    = bas_droit;
     }
 }
 void line_min3_ui16matrix_swp_rotation(uint16 **X16, int i, int j0, int j1, uint16 **Y16)
 // ------------------------------------------------------------------------
 {
     //charger la bordure de gauche
     uint16 haut_gauche   = 0;
     uint16 milieu_gauche = 0;
     uint16 bas_gauche    = 0;

     uint16 haut_milieu   = load2(X16, i-1 , j0);
     uint16 milieu_milieu = load2(X16, i   , j0);
     uint16 bas_milieu    = load2(X16, i+1 , j0);

     uint16 min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
     uint16 min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);

     uint16 haut_droit,  milieu_droit,  bas_droit, min3_droite;
     uint16 mot_pemutation_gauche , mot_permutation_droit;
     uint16 min3_res;
     // pas d'epilogue car utilisation du padding
     for (int j = j0; j<= j1; j++){
         haut_droit    = load2(X16, i-1,   j+1);
         milieu_droit  = load2(X16, i  ,   j+1);
         bas_droit     = load2(X16, i+1,   j+1);

         min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
         min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);
         min3_droite  = mot_min3(haut_droit,  milieu_droit,  bas_droit);

         //permutation
         mot_pemutation_gauche = macro_ui16left1(min3_gauche, min3_milieu);
         mot_permutation_droit = macro_ui16right1(min3_milieu, min3_droite);
         //min
         min3_res = mot_min3(mot_pemutation_gauche, min3_milieu, mot_permutation_droit);
         // printf("j : %d, r : %d\n", j , min3_res);
         //store
         store2(Y16, i, j, min3_res);
         //permutation colonne gauche
         haut_gauche   = haut_milieu;
         milieu_gauche = milieu_milieu;
         bas_gauche    = bas_milieu;

         haut_milieu   = haut_droit;
         milieu_milieu = milieu_droit;
         bas_milieu    = bas_droit;
     }
 }
 void line_min3_ui32matrix_swp_rotation(uint32 **X32, int i, int j0, int j1, uint32 **Y32)
 // ------------------------------------------------------------------------
 {
     //charger la bordure de gauche
     uint32 haut_gauche   = 0;
     uint32 milieu_gauche = 0;
     uint32 bas_gauche    = 0;

     uint32 haut_milieu   = load2(X32, i-1 , j0);
     uint32 milieu_milieu = load2(X32, i   , j0);
     uint32 bas_milieu    = load2(X32, i+1 , j0);

     uint32 min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
     uint32 min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);

     uint32 haut_droit,  milieu_droit,  bas_droit, min3_droite;
     uint32 mot_pemutation_gauche , mot_permutation_droit;
     uint32 min3_res;
     // pas d'epilogue car utilisation du padding
     for (int j = j0; j<= j1; j++){
         haut_droit    = load2(X32, i-1,   j+1);
         milieu_droit  = load2(X32, i  ,   j+1);
         bas_droit     = load2(X32, i+1,   j+1);

         min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
         min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);
         min3_droite  = mot_min3(haut_droit,  milieu_droit,  bas_droit);

         //permutation
         mot_pemutation_gauche = macro_ui32left1(min3_gauche, min3_milieu);
         mot_permutation_droit = macro_ui32right1(min3_milieu, min3_droite);
         //min
         min3_res = mot_min3(mot_pemutation_gauche, min3_milieu, mot_permutation_droit);
         // printf("j : %d, r : %d\n", j , min3_res);
         //store
         store2(Y32, i, j, min3_res);
         //permutation colonne gauche
         haut_gauche   = haut_milieu;
         milieu_gauche = milieu_milieu;
         bas_gauche    = bas_milieu;

         haut_milieu   = haut_droit;
         milieu_milieu = milieu_droit;
         bas_milieu    = bas_droit;
     }
 }
 // -------------------------------------------------------------------------------
 void min3_ui8matrix_swp_rotation(uint8 **X, int h, int w, int i0, int i1, int j0, int j1, uint8 **Z8, uint8 **Y)
 // -------------------------------------------------------------------------------
 {
     int r       = 2;
     int w8      = j1+1;
     uint8 **T8  = ui8matrix(0-1*r, h-1+1*r, 0-1*r, w8-1+1*r);
     pack_ui8matrix(X, h, w, T8);

     // displayR_ui8matrix (X8, i0, i1, j0, j1, "X8_mat");
     for(int i = i0; i<=i1; i++){
       line_min3_ui8matrix_swp_rotation(T8, i, j0, j1, Z8);
     }
     unpack_ui8matrix (Z8 , h, w8, Y);
 }
 // -------------------------------------------------------------------------------
 void min3_ui8matrix_swp_rotation_trivial(uint8 **X8, int h, int w, int i0, int i1, int j0, int j1, uint8 **Z8)
 // -------------------------------------------------------------------------------
 {
     // displayR_ui8matrix (X8, i0, i1, j0, j1, "X8_mat");
     for(int i = i0; i<=i1; i++){
       line_min3_ui8matrix_swp_rotation(X8, i, j0, j1, Z8);
     }
 }


 // -------------------------------------------------------------------------------
 void min3_ui16matrix_swp_rotation(uint8 **X, int h, int w, int i0, int i1, int j0, int j1, uint16 **Z16, uint8 **Y)
 // -------------------------------------------------------------------------------
 {
     int r         = 2;
     int w16       = j1+1;
     uint16 **T16  = ui16matrix(0-1*r, h-1+1*r, 0-1*r, w16-1+1*r);
     pack_ui16matrix(X, h, w, T16);
     // display_ui8matrix  (X,  0, h-1, 0, w-1, "%2d", "X");
     // display_ui16matrix (T16,  0, h-1, 0, j1, "%5d", "T8");
     // displayM_ui16matrix (T16, 0, i1, j0, j1, "16_mat");


     // displayR_ui8matrix (X8, i0, i1, j0, j1, "X8_mat");
     for(int i = i0; i<=i1; i++){
       line_min3_ui16matrix_swp_rotation(T16, i, j0, j1, Z16);
     }
     //displayR_ui16matrix (Y8, i0, i1, j0, j1, "Y8_mat");
     unpack_ui16matrix (Z16 , h, w16, Y);
 }
 // -------------------------------------------------------------------------------
 void min3_ui16matrix_swp_rotation_trivial(uint16 **X16, int h, int w, int i0, int i1, int j0, int j1, uint16 **Z16)
 // -------------------------------------------------------------------------------
 {
     // displayR_ui8matrix (X8, i0, i1, j0, j1, "X8_mat");
     for(int i = i0; i<=i1; i++){
       line_min3_ui16matrix_swp_rotation(X16, i, j0, j1, Z16);
     }
 }
 // -------------------------------------------------------------------------------
 void min3_ui32matrix_swp_rotation(uint8 **X, int h, int w, int i0, int i1, int j0, int j1, uint32 **Z32, uint8 **Y)
 // -------------------------------------------------------------------------------
 {
   if (Y[0] == NULL)
     {
         printf("Échec de l'allocation function\n");
         exit( EXIT_FAILURE);
     }
     int r         = 2;
     int w32       = j1+1;
     uint32 **T32  = ui32matrix(0-1*r, h-1+1*r, 0-1*r, w32-1+1*r);
     // printf("pack \n" );
     pack_ui32matrix(X, h, w, T32);
     // display_ui8matrix  (X,  0, h-1, 0, w-1, "%2d", "X");
     // display_ui16matrix (T16,  0, h-1, 0, j1, "%5d", "T8");
     // displayM_ui32matrix (T32, i0, i1, j0, j1, "T32_mat");

     // printf("boucle \n" );
     // displayR_ui8matrix (X8, i0, i1, j0, j1, "X8_mat");
     for(int i = i0; i<=i1; i++){
       line_min3_ui32matrix_swp_rotation(T32, i, j0, j1, Z32);
     }
     // displayM_ui32matrix (Z32, i0, i1, j0, j1, "Z32_mat");
     // printf("fin boucle \n" );

     unpack_ui32matrix (Z32 , h, w32, Y);
     // printf("unpack \n" );
     // display_ui8matrix  (Y,  0, h-1, 0, w-3, "%3d", "Y");

 }
 // -------------------------------------------------------------------------------
 void min3_ui32matrix_swp_rotation_trivial(uint32 **X32, int h, int w, int i0, int i1, int j0, int j1, uint32 **Z32)
 // -------------------------------------------------------------------------------
 {
     for(int i = i0; i<=i1; i++){
       line_min3_ui32matrix_swp_rotation(X32, i, j0, j1, Z32);
     }
 }
