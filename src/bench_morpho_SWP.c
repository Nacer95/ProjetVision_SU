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
#include <time.h>

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

#include "morpho_SWP_max.h"
#include "morpho_SWP_min.h"
#include "morpho_SWP_ouverture.h"



// ---------------------------------------------------
void bench_morpho_routine_swp(int n0, int n1, int nstep)
// ---------------------------------------------------
{
    int r = 1;
    int h = n1; // max size
    int w = n1; //max size
    int w0 = w;
    int w8 = w0 / 8; if(w0 % 8) w8 = w8+1;
    int w1 = 8 * w8; // w1 >= w

    //variable pour SWP
    int w16 = w0 / 16; if(w0 % 16) w16 = w16+1;
    int w32 = w0 / 32; if(w0 % 32) w32 = w32+1;
    int w1_8  = 8 * w8;
    int w1_16 = 16 * w16;
    int w1_32 = 32 * w32;

    uint8 **X;
    uint8 **X1;

    puts("========================================");
    puts("==       Ouverture du fichier        ===");
    puts("========================================");
    char * bench_max        = "bench_resultats/cpp/bench_max_tableau.txt"                  ;
    char * bench_max_s      = "bench_resultats/s/bench_max_tableau_secondes.txt"           ;
    char * bench_SWP_max    = "bench_resultats/cpp/bench_SWP_max_tableau.txt"              ;
    char * bench_SWP_max_s  = "bench_resultats/s/bench_SWP_max_tableau_secondes.txt"       ;
    char * bench_SWP_pipeline_ouverture     = "bench_resultats/cpp/bench_SWP_pipeline_ouverture.txt"       ;
    char * bench_SWP_pipeline_ouverture_s   = "bench_resultats/s/bench_SWP_pipeline_ouverture_seconde.txt"       ;




    FILE * inputFile_max;
    FILE * inputFile_max_s;
    FILE * inputFile_SWP_max;
    FILE * inputFile_SWP_max_s;
    FILE * inputFile_SWP_pipeline_ouverture;
    FILE * inputFile_SWP_pipeline_ouverture_s;


    inputFile_max = fopen( bench_max, "a" );
    if ( inputFile_max == NULL ) {
        fprintf( stderr, "Cannot open file inputFile_max \n" );
        exit( 0 );
    }

    inputFile_max_s = fopen( bench_max_s, "a" );
    if ( inputFile_max_s == NULL ) {
        fprintf( stderr, "Cannot open file inputFile_max_s \n" );
        exit( 0 );
    }

    inputFile_SWP_max = fopen( bench_SWP_max, "a" );
    if ( inputFile_SWP_max == NULL ) {
        fprintf( stderr, "Cannot open file inputFile_SWP_max \n" );
        exit( 0 );
    }

    inputFile_SWP_max_s = fopen( bench_SWP_max_s, "a" );
    if ( inputFile_SWP_max_s == NULL ) {
        fprintf( stderr, "Cannot open file inputFile_SWP_max \n" );
        exit( 0 );
    }
    inputFile_SWP_pipeline_ouverture = fopen( bench_SWP_pipeline_ouverture, "a" );
    if ( inputFile_SWP_pipeline_ouverture == NULL ) {
        fprintf( stderr, "Cannot open file inputFile_SWP_pipeline_ouverture \n" );
        exit( 0 );
    }
    inputFile_SWP_pipeline_ouverture_s = fopen( bench_SWP_pipeline_ouverture_s, "a" );
    if ( inputFile_SWP_pipeline_ouverture_s == NULL ) {
        fprintf( stderr, "Cannot open file inputFile_SWP_pipeline_ouverture_s \n" );
        exit( 0 );
    }

    puts("========================================");
    puts("==              Script               ===");
    puts("========================================");

    char * nom_graphe_max      = "(cycle/point) selon la taille des images (MAX-Dilatation)";
    char * nom_graphe_max_s    = "time selon la taille des images (MAX-Dilatation) seconde";
    char * nom_graphe_SWP_max  = "(cycle/point) selon la taille des images (SWP_max-Dilatation)";
    char * nom_graphe_SWP_max_s= "times selon la taille des images (SWP_max-Dilatation) seconde";
    char * nom_graphe_SWP_pipeline_ouverture   = "(cycle/point) selon la taille des images (SWP_max-Dilatation) seconde";
    char * nom_graphe_SWP_pipeline_ouverture_s = "time selon la taille des images (SWP_max-Dilatation) seconde";




    char * line_de_commande_max       = "python3 graphique.py bench_resultats/cpp/bench_max_tableau.txt cyclePpoint_selon_la_taille_des_images_MAXDilatation max cpp";
    char * line_de_commande_max_s     = "python3 graphique.py bench_resultats/s/bench_max_tableau_secondes.txt time_selon_la_taille_des_images_MAXDilatation_seconde max s";
    char * line_de_commande_SWP_MAX   = "python3 graphique.py bench_resultats/cpp/bench_SWP_max_tableau.txt cyclePpoint_selon_la_taille_des_images_SWPMAX_Dilatation swp_max cpp";
    char * line_de_commande_SWP_MAX_s = "python3 graphique.py bench_resultats/s/bench_SWP_max_tableau_secondes.txt time_selon_la_taille_des_images_SWPMAX_Dilatation_seconde swp_max s";
    char * line_de_commande_SWP_pipeline_ouverture    = "python3 graphique.py bench_resultats/cpp/bench_SWP_pipeline_ouverture.txt cyclePpoint_selon_la_taille_des_images_SWP_PIPELINE swp_pipeline cpp";
    char * line_de_commande_SWP_pipeline_ouverture_s  = "python3 graphique.py bench_resultats/s/bench_SWP_pipeline_ouverture_seconde.txt time_selon_la_taille_des_images_SWP_PIPELINE_seconde swp_pipeline s";


    // sprintf(line_de_commande_max,       "python3 graphique.py %s %s", bench_max     , nom_graphe_max      );
    // sprintf(line_de_commande_fusion,    "python3 graphique.py %s %s", bench_fusion  , nom_graphe_fusion   );
    // sprintf(line_de_commande_pipeline,  "python3 graphique.py %s %s", bench_pipeline, nom_graphe_pipeline );


    puts("========================================");
    puts("==      format d'affichage           ===");
    puts("========================================");
    char * format = "%8.2f";
    format = "%5.0f";
    format = "%6.4f";
    char * formatFichier = "%6.4f ";

    puts("========================================");
    puts("==    bench_morpho_max allocation    ===");
    puts("========================================");

    uint8 **Y_bas, **Y_reg, **Y_rot, **Y_red, **Y_ilu3, **Y_ilu3r, **Y_elu2r, **Y_elu2rf,**Y_ilu3_elu2r, **Y_ilu3_elu2rf;

    Y_bas         = ui8matrix(0, h-1, 0, w1-1);
    Y_reg         = ui8matrix(0, h-1, 0, w1-1);
    Y_rot         = ui8matrix(0, h-1, 0, w1-1);
    Y_red         = ui8matrix(0, h-1, 0, w1-1);
    Y_ilu3        = ui8matrix(0, h-1, 0, w1-1);
    Y_ilu3r       = ui8matrix(0, h-1, 0, w1-1);
    Y_elu2r       = ui8matrix(0, h-1, 0, w1-1);
    Y_elu2rf      = ui8matrix(0, h-1, 0, w1-1);
    Y_ilu3_elu2r  = ui8matrix(0, h-1, 0, w1-1);
    Y_ilu3_elu2rf = ui8matrix(0, h-1, 0, w1-1);

    zero_ui8matrix(Y_bas        , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_reg        , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_rot        , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_red        , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_ilu3       , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_ilu3r      , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_elu2r      , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_elu2rf     , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_ilu3_elu2r , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w1-1);

    double cpp_max_basic;
    double cpp_max_reg;
    double cpp_max_rot;
    double cpp_max_red;
    double cpp_max_ilu3;
    double cpp_max_ilu3_red;
    double cpp_max_elu2;
    double cpp_max_elu2_red;
    double cpp_max_elu2_red_factor;
    double cpp_max_ilu3_elu2_red;
    double cpp_max_ilu3_elu2_red_factor;
    double cpp_min_ilu3_elu2_red_factor;
    double cpp_ouverture_min_puis_max = 0;


    double s_max_basic;
    double s_max_reg;
    double s_max_rot;
    double s_max_red;
    double s_max_ilu3;
    double s_max_ilu3_red;
    double s_max_elu2;
    double s_max_elu2_red;
    double s_max_elu2_red_factor;
    double s_max_ilu3_elu2_red;
    double s_max_ilu3_elu2_red_factor;
    double s_min_ilu3_elu2_red_factor;
    double s_ouverture_min_puis_max = 0;


    puts("============================================");
    puts("==    bench_morpho_SWP_max allocation    ===");
    puts("============================================");


    uint8 **Y8_rotation, **Y8_rotation_unpack, **Y16_rotation_unpack, **Y32_rotation_unpack;
    uint16 **Y16_rotation;
    uint32 **Y32_rotation;
    uint8  **X8   = ui8matrix (0-2*r, h-1+2*r, 0-2*r, w8-1+2*r);
    uint16 **X16  = ui16matrix(0-1*r, h-1+1*r, 0-1*r, w16-1+1*r);
    uint32 **X32  = ui32matrix(0-1*r, h-1+1*r, 0-1*r, w32-1+1*r);

    Y8_rotation                        = ui8matrix (0-2*r, h-1+2*r, 0-2*r,  w8-1+2*r);
    Y16_rotation                       = ui16matrix(0-2*r, h-1+2*r, 0-1*r, w16-1+1*r);
    Y32_rotation                       = ui32matrix(0-2*r, h-1+2*r, 0-1*r, w32-1+1*r);

    Y8_rotation_unpack                 = ui8matrix(0, h-1, 0, w1_8-1);
    Y16_rotation_unpack                = ui8matrix(0, h-1, 0, w1_16-1);
    Y32_rotation_unpack                = ui8matrix(0, h-1, 0, w1_32-1);

    zero_ui8matrix (Y8_rotation,    0-2*r, h-1+2*r, 0-1*r, w8-2+2*r);
    zero_ui16matrix(Y16_rotation,   0-2*r, h-1+2*r, 0-1*r, w16-1+1*r);
    zero_ui32matrix(Y32_rotation,   0-2*r, h-1+2*r, 0-1*r, w32-1+1*r);

    zero_ui8matrix( Y8_rotation_unpack,      0-0*r, h-1+0*r, 0-0*r, w1_8-1+0*r);
    zero_ui8matrix(Y16_rotation_unpack,      0-0*r, h-1+0*r, 0-0*r, w1_16-1+0*r);
    zero_ui8matrix(Y32_rotation_unpack,      0-0*r, h-1+0*r, 0-0*r, w1_32-1+0*r);

    double cpp_SWP_max_rot_uint8;
    double cpp_SWP_max_rot_uint16;
    double cpp_SWP_max_rot_uint32;

    double s_SWP_max_rot_uint8;
    double s_SWP_max_rot_uint16;
    double s_SWP_max_rot_uint32;

    puts("=================================================");
    puts("==    bench_morpho_SWP_pipeline allocation    ===");
    puts("=================================================");
    uint8 **Y_basic, **T_basic, **T8_rotation;
    uint8 **Y_fusion_ilu5_elu2_red_factor;
    uint8 **T_pipeline_ilu3_elu2_red_factor   , **Y_pipeline_ilu3_elu2_red_factor;

    T8_rotation                     = ui8matrix (0-2*r, h-1+2*r, 0-2*r,  w8-1+2*r);
    Y_fusion_ilu5_elu2_red_factor   = ui8matrix(0, h-1, 0, w1-1)                       ;
    T_pipeline_ilu3_elu2_red_factor = ui8matrix(0-1*r, h-1+1*r, 0-1*r, w1-1+1*r)       ;
    Y_pipeline_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1)                       ;
    T_basic                         = ui8matrix (0-2*r, h-1+2*r, 0-1*r,  w1_8-1+1*r)   ;
    Y_basic                         = ui8matrix (0-2*r, h-1+2*r, 0-1*r,  w1_8-1+1*r)   ;

    zero_ui8matrix (T8_rotation,    0-2*r, h-1+2*r, 0-1*r, w8-1+1*r);
    zero_ui8matrix(Y_fusion_ilu5_elu2_red_factor, 0-0*r, h-1+0*r, 0-0*r, w1-1+0*r)      ;
    zero_ui8matrix(T_pipeline_ilu3_elu2_red_factor     , 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r)    ;
    zero_ui8matrix(Y_pipeline_ilu3_elu2_red_factor          , 0-0*r, h-1+0*r, 0-0*r, w1-1+0*r)    ;

    double cpp_ouverture_basic;
    double cpp_SWP_pipeline_rot_uint8;
    double cpp_fusion_ilu5_elu2_red_factor;
    double cpp_pipeline_ilu3_elu2_red_factor;

    double s_ouverture_basic;
    double s_SWP_pipeline_rot_uint8;
    double s_fusion_ilu5_elu2_red_factor;
    double s_pipeline_ilu3_elu2_red_factor;


    // puts("malloc");
    // X 2r-border
    X      = ui8matrix(0-2*r, h-1+2*r, 0-2*r, w1-1+2*r);
    X8     = ui8matrix (0-2*r, h-1+2*r, 0-2*r,  w8-1+2*r);


    //PUTS("zero X");

    puts("temps de calcul en ccp (cycle/point)");


    for(int n = n0; n <= n1; n += nstep) {
        // printf("entree\n");
        h = n;
        w1 = n / 1;
        w8 =  n / 8; if(n % 8) w8 = w8+1;
        w16 = n / 16; if(n % 16) w16 = w16+1;
        w32 = n / 32; if(n % 32) w32 = w32+1;

        w1_8  = 8 * w8;
        w1_16 = 16 * w16;
        w1_32 = 32 * w32;

        //printf("i = %3d\n", n);
        resize_ui8matrix(X , 0-2*r, h-1+2*r, 0-2*r, w1_32-1+2*r);

        // puts("==========================================");
        // puts("==    bench_morpho_max reallocation    ===");
        // puts("==========================================");

        resize_ui8matrix(Y_bas        , 0, h-1, 0, w1-1);
        resize_ui8matrix(Y_reg        , 0, h-1, 0, w1-1);
        resize_ui8matrix(Y_rot        , 0, h-1, 0, w1-1);
        resize_ui8matrix(Y_red        , 0, h-1, 0, w1-1);
        resize_ui8matrix(Y_ilu3       , 0, h-1, 0, w1-1);
        resize_ui8matrix(Y_ilu3r      , 0, h-1, 0, w1-1);
        resize_ui8matrix(Y_elu2r      , 0, h-1, 0, w1-1);
        resize_ui8matrix(Y_elu2rf     , 0, h-1, 0, w1-1);
        resize_ui8matrix(Y_ilu3_elu2r , 0, h-1, 0, w1-1);
        resize_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w1-1);

        // puts("==============================================");
        // puts("==    bench_morpho_SWP_max reallocation    ===");
        // puts("==============================================");

        resize_ui8matrix (Y8_rotation,    0-2*r, h-1+2*r, 0-2*r, w8-1+2*r);
        resize_ui16matrix(Y16_rotation,   0-2*r, h-1+2*r, 0-1*r, w16-1+1*r);
        resize_ui32matrix(Y32_rotation,   0-2*r, h-1+2*r, 0-1*r, w32-1+1*r);

        resize_ui8matrix( Y8_rotation_unpack,      0-0*r, h-1+0*r, 0-0*r, w1_8-1+0*r);
        resize_ui8matrix(Y16_rotation_unpack,      0-0*r, h-1+0*r, 0-0*r, w1_16-1+0*r);
        resize_ui8matrix(Y32_rotation_unpack,      0-0*r, h-1+0*r, 0-0*r, w1_32-1+0*r);

        // puts("===================================================");
        // puts("==    bench_morpho_SWP_pipeline reallocation    ===");
        // puts("===================================================");
        resize_ui8matrix(T_basic                        ,   0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
        resize_ui8matrix(Y_basic                        ,   0, h-1, 0, w1-1);
        resize_ui8matrix(T8_rotation                    ,   0-2*r, h-1+2*r, 0-2*r, w8-1+2*r);
        resize_ui8matrix(T_pipeline_ilu3_elu2_red_factor,   0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
        resize_ui8matrix(Y_fusion_ilu5_elu2_red_factor  ,   0, h-1, 0, w1-1);
        resize_ui8matrix(Y_pipeline_ilu3_elu2_red_factor,   0, h-1, 0, w1-1);

        // puts("====================================");
        // puts("==        Bench référence        ===");
        // puts("====================================");
        BENCH(max3_ui8matrix_basic               (X, 0, h-1, 0, w0-1, Y_bas        ), n, cpp_max_basic);
        BENCH_secondes(max3_ui8matrix_basic      (X, 0, h-1, 0, w0-1, Y_bas        ), n, s_max_basic);

        BENCH(ouverture3_ui8matrix_basic          (X, 0, h-1, 0, w0-1, T_basic,  Y_basic  ), n,  cpp_ouverture_basic) ;
        BENCH_secondes(ouverture3_ui8matrix_basic (X, 0, h-1, 0, w0-1, T_basic,  Y_basic  ), n,  s_ouverture_basic) ;



        // puts("======================================");
        // puts("==    bench_morpho_SWP_max TEST    ===");
        // puts("======================================");

        // BENCH(max3_ui8matrix_swp_rotation (X, h, w1_8, 0, h-1, 0, w8-1,  Y8_rotation,   Y8_rotation_unpack) , n , cpp_SWP_max_rot_uint8) ;
        // BENCH(max3_ui16matrix_swp_rotation(X, h, w1_8, 0, h-1, 0, w16-1, Y16_rotation,  Y16_rotation_unpack), n , cpp_SWP_max_rot_uint16);
        // BENCH(max3_ui32matrix_swp_rotation(X, h, w1_8, 0, h-1, 0, w32-1, Y32_rotation,  Y32_rotation_unpack), n , cpp_SWP_max_rot_uint32);
        //
        // BENCH_secondes(max3_ui8matrix_swp_rotation (X, h, w1_8, 0, h-1, 0, w8-1,  Y8_rotation,   Y8_rotation_unpack) , n , s_SWP_max_rot_uint8) ;
        // BENCH_secondes(max3_ui16matrix_swp_rotation(X, h, w1_8, 0, h-1, 0, w16-1, Y16_rotation,  Y16_rotation_unpack), n , s_SWP_max_rot_uint16);
        // BENCH_secondes(max3_ui32matrix_swp_rotation(X, h, w1_8, 0, h-1, 0, w32-1, Y32_rotation,  Y32_rotation_unpack), n , s_SWP_max_rot_uint32);
        resize_ui8matrix (X8,    0-2*r, h-1+2*r, 0-2*r, w8-1+2*r);
        resize_ui16matrix(X16,   0-1*r, h-1+1*r, 0-1*r, w16-1+1*r);
        resize_ui32matrix(X32,   0-1*r, h-1+1*r, 0-1*r, w32-1+1*r);
        //
        pack_ui8matrix (X, h, w1_8, X8);
        pack_ui16matrix(X, h, w1_8, X16);
        pack_ui32matrix(X, h, w1_8, X32);

        BENCH(max3_ui8matrix_swp_rotation_trivial (X8,  h, w1_8, 0, h-1, 0, w8-1,  Y8_rotation) , n , cpp_SWP_max_rot_uint8) ;
        BENCH(max3_ui16matrix_swp_rotation_trivial(X16, h, w1_8, 0, h-1, 0, w16-1, Y16_rotation), n , cpp_SWP_max_rot_uint16);
        BENCH(max3_ui32matrix_swp_rotation_trivial(X32, h, w1_8, 0, h-1, 0, w32-1, Y32_rotation), n , cpp_SWP_max_rot_uint32);

        BENCH_secondes(max3_ui8matrix_swp_rotation_trivial (X8, h, w1_8, 0, h-1, 0, w8-1,   Y8_rotation) , n , s_SWP_max_rot_uint8) ;
        BENCH_secondes(max3_ui16matrix_swp_rotation_trivial(X16, h, w1_8, 0, h-1, 0, w16-1, Y16_rotation), n , s_SWP_max_rot_uint16);
        BENCH_secondes(max3_ui32matrix_swp_rotation_trivial(X32, h, w1_8, 0, h-1, 0, w32-1, Y32_rotation), n , s_SWP_max_rot_uint32);

        // free_ui8matrix (X8,                        0-1*r, h-1+1*r, 0-1*r, w8-1+1*r );
        // free_ui16matrix(X16,                       0-1*r, h-1+1*r, 0-1*r, w16-1+1*r );
        // free_ui32matrix(X32,                       0-1*r, h-1+1*r, 0-1*r, w32-1+1*r );


        // unpack_ui8matrix  (Y8_rotation   , h, w8,  Y8_rotation_unpack);
        // unpack_ui16matrix (Y16_rotation  , h, w16, Y16_rotation_unpack);
        // unpack_ui32matrix (Y32_rotation  , h, w32, Y32_rotation_unpack);

        // puts("==================================");
        // puts("==    bench_morpho_max TEST    ===");
        // puts("==================================");

        BENCH(max3_ui8matrix_reg                 (X, 0, h-1, 0, w0-1, Y_reg        ), n, cpp_max_reg);
        BENCH(max3_ui8matrix_rot                 (X, 0, h-1, 0, w0-1, Y_rot        ), n, cpp_max_rot);
        BENCH(max3_ui8matrix_red                 (X, 0, h-1, 0, w0-1, Y_red        ), n, cpp_max_red);
        BENCH(max3_ui8matrix_ilu3                (X, 0, h-1, 0, w0-1, Y_ilu3       ), n, cpp_max_ilu3);
        BENCH(max3_ui8matrix_ilu3_red            (X, 0, h-1, 0, w0-1, Y_ilu3r      ), n, cpp_max_ilu3_red);
        BENCH(max3_ui8matrix_elu2_red            (X, 0, h-1, 0, w0-1, Y_elu2r      ), n, cpp_max_elu2_red);
        BENCH(max3_ui8matrix_elu2_red_factor     (X, 0, h-1, 0, w0-1, Y_elu2rf     ), n, cpp_max_elu2_red_factor);
        BENCH(max3_ui8matrix_ilu3_elu2_red       (X, 0, h-1, 0, w0-1, Y_ilu3_elu2r ), n, cpp_max_ilu3_elu2_red);
        BENCH(max3_ui8matrix_ilu3_elu2_red_factor(X, 0, h-1, 0, w0-1, Y_ilu3_elu2rf), n, cpp_max_ilu3_elu2_red_factor);
        // BENCH(min3_ui8matrix_ilu3_elu2_red_factor(X, 0, h-1, 0, w0-1, Y_ilu3_elu2rf), n, cpp_min_ilu3_elu2_red_factor);
        // cpp_ouverture_min_puis_max = cpp_max_ilu3_elu2_red_factor + cpp_min_ilu3_elu2_red_factor;


        BENCH_secondes(max3_ui8matrix_reg                 (X, 0, h-1, 0, w0-1, Y_reg        ), n, s_max_reg);
        BENCH_secondes(max3_ui8matrix_rot                 (X, 0, h-1, 0, w0-1, Y_rot        ), n, s_max_rot);
        BENCH_secondes(max3_ui8matrix_red                 (X, 0, h-1, 0, w0-1, Y_red        ), n, s_max_red);
        BENCH_secondes(max3_ui8matrix_ilu3                (X, 0, h-1, 0, w0-1, Y_ilu3       ), n, s_max_ilu3);
        BENCH_secondes(max3_ui8matrix_ilu3_red            (X, 0, h-1, 0, w0-1, Y_ilu3r      ), n, s_max_ilu3_red);
        BENCH_secondes(max3_ui8matrix_elu2_red            (X, 0, h-1, 0, w0-1, Y_elu2r      ), n, s_max_elu2_red);
        BENCH_secondes(max3_ui8matrix_elu2_red_factor     (X, 0, h-1, 0, w0-1, Y_elu2rf     ), n, s_max_elu2_red_factor);
        BENCH_secondes(max3_ui8matrix_ilu3_elu2_red       (X, 0, h-1, 0, w0-1, Y_ilu3_elu2r ), n, s_max_ilu3_elu2_red);
        BENCH_secondes(max3_ui8matrix_ilu3_elu2_red_factor(X, 0, h-1, 0, w0-1, Y_ilu3_elu2rf), n, s_max_ilu3_elu2_red_factor);


        // puts("============================================");
        // puts("==    bench_morpho_ouverture SWP TEST    ===");
        // puts("============================================");

        BENCH(ouverture3_ui8matrix_pipeline_SWP_rotation_trivial          (X8, h, w8, 0, h-1, 0, w8-1,  T8_rotation, Y8_rotation                                          ), n,  cpp_SWP_pipeline_rot_uint8);
        // printf("a\n");
        BENCH(ouverture3_ui8matrix_fusion_ilu5_elu2_red_factor            (X, 0, h-1, 0, w1_32-1,                                            Y_fusion_ilu5_elu2_red_factor   ), n,  cpp_fusion_ilu5_elu2_red_factor );
        BENCH(ouverture3_ui8matrix_pipeline_elu2_red_factor               (X, 0, h-1, 0, w1_32-1,          T_pipeline_ilu3_elu2_red_factor      , Y_pipeline_ilu3_elu2_red_factor      ), n,  cpp_pipeline_ilu3_elu2_red_factor    );
        // printf("b\n");

        // BENCH_secondes(ouverture3_ui8matrix_basic                         (X, 0, h-1, 0, w0-1, T_basic,  Y_basic                                                          ), n, s_ouverture_basic) ;
        BENCH_secondes(ouverture3_ui8matrix_pipeline_SWP_rotation_trivial (X8, h, w8, 0, h-1, 0, w8-1,  T8_rotation, Y8_rotation                                          ), n, s_SWP_pipeline_rot_uint8);
        BENCH_secondes(ouverture3_ui8matrix_fusion_ilu5_elu2_red_factor   (X, 0, h-1, 0, w1_32-1,                                            Y_fusion_ilu5_elu2_red_factor   ), n, s_fusion_ilu5_elu2_red_factor   );
        // printf("c\n");

        BENCH_secondes(ouverture3_ui8matrix_pipeline_ilu3_elu2_red_factor      (X, 0, h-1, 0, w1_32-1,          T_pipeline_ilu3_elu2_red_factor      , Y_pipeline_ilu3_elu2_red_factor      ), n, s_pipeline_ilu3_elu2_red_factor      );


        /**/
        printf("i = %4d\n", n);


        // puts("=======================================");
        // puts("==    bench_morpho_max RESULTATS    ===");
        // puts("=======================================");
        // printf("   ");
        //
        // printf(format, cpp_max_basic                      );
        // printf(format, cpp_max_reg                        );
        // printf(format, cpp_max_rot                        );
        // printf(format, cpp_max_red                        );
        // printf(format, cpp_max_ilu3_red                   );
        // printf(format, cpp_max_elu2_red                   );
        // printf(format, cpp_max_elu2_red_factor            );
        // printf(format, cpp_max_ilu3_elu2_red              );
        // printf(format, cpp_max_ilu3_elu2_red_factor       );

        // puts("===========================================");
        // puts("==    bench_SWP_morpho_max RESULTATS    ===");
        // puts("===========================================");
        // printf("   ");
        //
        // printf(format, cpp_SWP_max_rot_uint8                        );
        // printf(format, cpp_SWP_max_rot_uint16                       );
        // printf(format, cpp_SWP_max_rot_uint32                       );
        //
        // putchar('\n');


        puts("========================================");
        puts("==    bench_morpho_max -> FICHIER    ===");
        puts("========================================");
        //
        fprintf( inputFile_max, "%d %6.1f %6.1f %6.1f %6.1f %6.1f %6.1f %6.1f %6.1f %6.1f\n",
        n,cpp_max_basic, cpp_max_reg, cpp_max_rot, cpp_max_red, cpp_max_ilu3_red, cpp_max_elu2_red,
        cpp_max_elu2_red_factor, cpp_max_ilu3_elu2_red, cpp_max_ilu3_elu2_red_factor);

        // printf("a\n");
        fprintf(inputFile_max_s,   "%d " ,     n                            );
        fprintf(inputFile_max_s, "%0.8f ", s_max_basic                      );
        fprintf(inputFile_max_s, "%0.8f ", s_max_reg                        );
        fprintf(inputFile_max_s, "%0.8f ", s_max_rot                        );
        fprintf(inputFile_max_s, "%0.8f ", s_max_red                        );
        fprintf(inputFile_max_s, "%0.8f ", s_max_ilu3_red                   );
        fprintf(inputFile_max_s, "%0.8f ", s_max_elu2_red                   );
        fprintf(inputFile_max_s, "%0.8f ", s_max_elu2_red_factor            );
        fprintf(inputFile_max_s, "%0.8f ", s_max_ilu3_elu2_red              );
        fprintf(inputFile_max_s, "%0.8f ", s_max_ilu3_elu2_red_factor       );
        fprintf(inputFile_max_s,  "\n"                                      );


        // //printf("b\n");

        puts("========================================");
        puts("==  bench_SWP_morpho_max -> FICHIER  ===");
        puts("========================================");
        fprintf(inputFile_SWP_max,     "%d ",         n                         );
        fprintf(inputFile_SWP_max,formatFichier, cpp_max_basic                  );
        fprintf(inputFile_SWP_max,formatFichier, cpp_SWP_max_rot_uint8          );
        fprintf(inputFile_SWP_max,formatFichier, cpp_SWP_max_rot_uint16         );
        fprintf(inputFile_SWP_max,formatFichier, cpp_SWP_max_rot_uint32         );
        fprintf(inputFile_SWP_max,     "\n"                                     );
        // printf("c\n");

        fprintf(inputFile_SWP_max_s,   "%d ",      n                       );
        fprintf(inputFile_SWP_max_s,"%0.8f ", s_max_basic                  );
        fprintf(inputFile_SWP_max_s,"%0.8f ", s_SWP_max_rot_uint8          );
        fprintf(inputFile_SWP_max_s,"%0.8f ", s_SWP_max_rot_uint16         );
        fprintf(inputFile_SWP_max_s,"%0.8f ", s_SWP_max_rot_uint32         );
        fprintf(inputFile_SWP_max_s,     "\n"                              );
        printf("d\n");

        puts("========================================");
        puts("==  bench_SWP_Pipeline -> FICHIER  ===");
        puts("========================================");


        fprintf(inputFile_SWP_pipeline_ouverture,     "%d ",         n                            );
        fprintf(inputFile_SWP_pipeline_ouverture,formatFichier, cpp_ouverture_basic               ); //enlever pour mieux visualiser // range de 1 le faire passer a 2 pour ne prendre qu'a partir de la seconde colonne
        fprintf(inputFile_SWP_pipeline_ouverture,formatFichier, cpp_SWP_pipeline_rot_uint8        );
        fprintf(inputFile_SWP_pipeline_ouverture,formatFichier, cpp_fusion_ilu5_elu2_red_factor   );
        fprintf(inputFile_SWP_pipeline_ouverture,formatFichier, cpp_pipeline_ilu3_elu2_red_factor );
        fprintf(inputFile_SWP_pipeline_ouverture,     "\n"                                        );
        // printf("c\n");

        // décommenter soit 1 et 2
        // 1 le temps de traitement d'une image entière
        // fprintf(inputFile_SWP_pipeline_ouverture_s,   "%d ",      n                                   );
        // fprintf(inputFile_SWP_pipeline_ouverture_s,"%0.8f ", s_ouverture_basic                        );
        // fprintf(inputFile_SWP_pipeline_ouverture_s,"%0.8f ", s_SWP_pipeline_rot_uint8                 );
        // fprintf(inputFile_SWP_pipeline_ouverture_s,"%0.8f ", s_fusion_ilu5_elu2_red_factor            );
        // fprintf(inputFile_SWP_pipeline_ouverture_s,"%0.8f ", s_pipeline_ilu3_elu2_red_factor          );
        // fprintf(inputFile_SWP_pipeline_ouverture_s,     "\n"                                          );

        // 2 le débit
        // fprintf(inputFile_SWP_pipeline_ouverture_s,   "%d ",      n                                           );
        // fprintf(inputFile_SWP_pipeline_ouverture_s,"%0.8f ", (n*n) / s_ouverture_basic                        );
        // fprintf(inputFile_SWP_pipeline_ouverture_s,"%0.8f ", (n*n) / s_SWP_pipeline_rot_uint8                 );
        // fprintf(inputFile_SWP_pipeline_ouverture_s,"%0.8f ", (n*n) / s_fusion_ilu5_elu2_red_factor            );
        // fprintf(inputFile_SWP_pipeline_ouverture_s,"%0.8f ", (n*n) / s_pipeline_ilu3_elu2_red_factor          );
        // fprintf(inputFile_SWP_pipeline_ouverture_s,     "\n"                                                  );
        // printf("d\n");


    }

    fclose(  inputFile_max        );
    fclose(  inputFile_max_s      );
    fclose(  inputFile_SWP_max    );
    fclose(  inputFile_SWP_max_s  );
    fclose(  inputFile_SWP_pipeline_ouverture  );
    fclose(  inputFile_SWP_pipeline_ouverture_s  );


    // system( line_de_commande_max        );
    // system( line_de_commande_max_s      );
    // system( line_de_commande_SWP_MAX    );
    // system( line_de_commande_SWP_MAX_s  );
    // system( line_de_commande_SWP_pipeline_ouverture     );
    // system( line_de_commande_SWP_pipeline_ouverture_s  );


}


int bench_morpho_swp(int argc, char* argv[]){
  bench_morpho_routine_swp(128, 3072, 16);

  // bench_morpho_routine_swp(128, 4096, 32);
  // bench_morpho_routine_swp(512, 2048, 16);
  return 0;
}
