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

/*
  (3 x 3) -> (1 x 1 ) ⸰ (3 x 3) -> (1 x 1 )
  Transférer une matrice de (3 x 3) déjà érodée qui se dilatée.
  Question: Combien de point traitées par min ?
  25 points étudiés pour transférer une matrice 3 x 3

  for (int j = j0; j<=j0; j++ ){
    Y[i] = 𝝈 (𝜖 (X[j]))
  }
  𝜖(X[j]) : (5 x 5) -> (3 x 3)

  ======== Trouvé Y[j] avec la fusion des opérateurs  ===========

                        b1  b2  b3  b4  b5
                        b6  a   b   c   b7
                i  ->   b8  d  X[j] f   b9
                        b10 g   h   i   b11
                        b12 b13 b14 b15 b16

                                |
              érosion appeler la fonction min pour 9 points
                                V

                            a'  b'  c'
                   i  ->    d'  e'  f'
                            g'  h'  i'

                                |
            dilatation appeler la fonction max pour 1 point
                                V

                              Y[j]

          Etapes :
            Solution 1 : pas d'appelle de fonction
              - Récupérer 25 point au tour de X[j]
                b1,   b2,   b3,   b4,  b5
                b6,                    b7
                b8,                    b9
                b10,                  b11
                b12, b13,   b14, b15, b16

                haut_gauche     haut_milieu       haut_droit
                milieu_gauche      X[j]           milieu_droit
                bas_gauche      bas_milieu        bas_droit

              - Faire une érosion sur abc dX[j]f ghi
              - Dilater le point

              - Version regression -> pas d'optimisation
            Solution 2 :
              - Appeler line_min3_ui8matrix_basic pour 9 point
              - Appeler la fonction line_max3_ui8matrix_basic pour 1 point
*/


// -------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------

{
  uint8 b1,     b2,       b3,       b4,         b5;
  uint8 b6,                                     b7;
  uint8 b8,                                     b9;
  uint8 b10,                                    b11;
  uint8 b12,   b13,      b14,        b15,       b16;

  uint8 haut_gauche,    haut_milieu,      haut_droit;
  uint8 milieu_gauche,  milieu_milieu,    milieu_droit;
  uint8 bas_gauche,     bas_milieu,       bas_droit;

  uint8 min_Col1, min_Col2, min_Col3, min_Col4, min_Col5;
  uint8 min_res;


  for (int j=j0; j<= j1; j++){

    //============ LOAD ==================
    b1 =load2(X, i-2, j-2);      b2 =load2(X, i-2, j-1);     b3 =load2(X, i-2, j);    b4 =load2(X, i-2, j+1);    b5 =load2(X, i-2, j+2);
    b6 =load2(X, i-1, j-2);                                                                                      b7 =load2(X, i-1, j+2);
    b8 =load2(X, i,   j-2);                                                                                      b9 =load2(X, i,   j+2);
    b10=load2(X, i+1, j-2);                                                                                      b11=load2(X, i+1, j+2);
    b12=load2(X, i+2, j-2);      b13=load2(X, i+2, j-1);     b14=load2(X, i+2, j);    b15=load2(X, i+2, j+1);    b16=load2(X, i+2, j+2);

                    haut_gauche  =load2(X, i-1, j-1);    haut_milieu=load2(X, i-1, j);      haut_droit=load2(X, i-1, j+1);
                    milieu_gauche=load2(X, i, j-1);      milieu_milieu=load2(X, i, j);      milieu_droit=load2(X, i, j+1);
                    bas_gauche   =load2(X, i+1, j-1);    bas_milieu=load2(X, i+1, j);       bas_droit=load2(X, i+1, j+1);

    //============ Calcul ==================
    //EROSION
    /*
      Mettre à jours les pixels suivant :
       haut_gauche,    haut_milieu,      haut_droit;
       milieu_gauche,  milieu_milieu,    milieu_droit;
       bas_gauche,     bas_milieu,       bas_droit;
    */

    // COLONNE DE GAUCHE =======================================================
    // MAJ haut_gauche
    uint8 haut_gauche_c1 = min3(b1, b6, b8);
    uint8 haut_gauche_c2 = min3(b2, haut_gauche, milieu_gauche);
    uint8 haut_gauche_c3 = min3(b3, haut_milieu, milieu_milieu);
    uint8 erosion_haut_gauche = min3(haut_gauche_c1, haut_gauche_c2, haut_gauche_c3);

    // MAJ milieu_gauche
    uint8 milieu_gauche_c1 = min3(b6, b8, b10);
    uint8 milieu_gauche_c2 = min3(haut_gauche, milieu_gauche, bas_gauche);
    uint8 milieu_gauche_c3 = min3(haut_milieu, milieu_milieu, bas_milieu);
    uint8 erosion_milieu_gauche = min3(milieu_gauche_c1, milieu_gauche_c2, milieu_gauche_c3);

    //MAJ bas_gauche
    uint8 bas_gauche_c1 = min3(b8, b10, b12);
    uint8 bas_gauche_c2 = min3(milieu_gauche, bas_gauche, b13);
    uint8 bas_gauche_c3 = min3(milieu_milieu, bas_milieu, b14);
    uint8 erosion_bas_gauche = min3(bas_gauche_c1, bas_gauche_c2, bas_gauche_c3);


    // COLONNE DU MILIEU =======================================================
    // MAJ haut_milieu
    uint8 haut_milieu_c1 = min3(b2, haut_gauche, milieu_gauche);
    uint8 haut_milieu_c2 = min3(b3, haut_milieu, milieu_milieu);
    uint8 haut_milieu_c3 = min3(b4, haut_droit,  milieu_droit);
    uint8 erosion_haut_milieu = min3(haut_milieu_c1, haut_milieu_c2, haut_milieu_c3);

    // MAJ milieu_milieu
    uint8 milieu_milieu_c1 = min3(haut_gauche, milieu_gauche, bas_gauche);
    uint8 milieu_milieu_c2 = min3(haut_milieu, milieu_milieu, bas_milieu);
    uint8 milieu_milieu_c3 = min3(haut_droit,  milieu_droit , bas_droit);
    uint8 erosion_milieu_milieu = min3(milieu_milieu_c1, milieu_milieu_c2, milieu_milieu_c3);

    // MAJ bas_milieu
    uint8 bas_milieu_c1 = min3(milieu_gauche, bas_gauche, b13);
    uint8 bas_milieu_c2 = min3(milieu_milieu, bas_milieu, b14);
    uint8 bas_milieu_c3 = min3(milieu_droit,  bas_droit,  b15);
    uint8 erosion_bas_milieu = min3(bas_milieu_c1, bas_milieu_c2, bas_milieu_c3);


    // COLONNE DROITE ==========================================================
    // MAJ haut_droit
    uint8 haut_droit_c1 = min3(b3, haut_milieu, milieu_milieu);
    uint8 haut_droit_c2 = min3(b4, haut_droit,  milieu_droit);
    uint8 haut_droit_c3 = min3(b5, b7, b9);
    uint8 erosion_haut_droit = min3(haut_droit_c1, haut_droit_c2, haut_droit_c3);

    // MAJ milieu_droit
    uint8 milieu_droit_c1 = min3(haut_milieu, milieu_milieu, bas_milieu);
    uint8 milieu_droit_c2 = min3(haut_droit,  milieu_droit,  bas_droit);
    uint8 milieu_droit_c3 = min3(b7, b9, b11);
    uint8 erosion_milieu_droit = min3(milieu_droit_c1, milieu_droit_c2, milieu_droit_c3);

    // MAJ bas_droit
    uint8 bas_droit_c1 = min3(milieu_milieu, bas_milieu, b14);
    uint8 bas_droit_c2 = min3(milieu_droit,  bas_droit,  b15);
    uint8 bas_droit_c3 = min3(b9, b11, b16);
    uint8 erosion_bas_droit = min3(bas_droit_c1, bas_droit_c2, bas_droit_c3);


    //Dilatation du point i,j
    uint8 dilatation_c1 = max3(erosion_haut_gauche, erosion_milieu_gauche, erosion_bas_gauche);
    uint8 dilatation_c2 = max3(erosion_haut_milieu, erosion_milieu_milieu, erosion_bas_milieu);
    uint8 dilatation_c3 = max3(erosion_haut_droit,  erosion_milieu_droit,  erosion_bas_droit);
    uint8 dilatation_res = max3(dilatation_c1, dilatation_c2, dilatation_c3);

    //============ Store ==================
    //Charger la valeur dans Y
    store2(Y, i, j, dilatation_res);

  }// fin de la boucle for



}
// ----------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_ilu5_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------------
/*
  reduction : réduire le nombre d'opération, minimiser le nombre de chargement d'information
*/
{


  uint8 b1,     b2,       b3,       b4,         b5;
  uint8 b6,                                     b7;
  uint8 b8,                                     b9;
  uint8 b10,                                    b11;
  uint8 b12,   b13,      b14,        b15,       b16;

  uint8 haut_gauche,    haut_milieu,      haut_droit;
  uint8 milieu_gauche,  milieu_milieu,    milieu_droit;
  uint8 bas_gauche,     bas_milieu,       bas_droit;


  b1 =load2(X, i-2, j0-2);      b2 =load2(X, i-2, j0-1);             b3 =load2(X, i-2, j0);             b4 =load2(X, i-2, j0+1);
  b6 =load2(X, i-1, j0-2);
  b8 =load2(X, i,   j0-2);
  b10=load2(X, i+1, j0-2);
  b12=load2(X, i+2, j0-2);      b13=load2(X, i+2, j0-1);            b14=load2(X, i+2, j0);             b15=load2(X, i+2, j0+1);

                          haut_gauche  =load2(X, i-1, j0-1);    haut_milieu=load2(X, i-1, j0);      haut_droit=load2(X, i-1, j0+1);
                          milieu_gauche=load2(X, i, j0-1);      milieu_milieu=load2(X, i, j0);      milieu_droit=load2(X, i, j0+1);
                          bas_gauche   =load2(X, i+1, j0-1);    bas_milieu=load2(X, i+1, j0);       bas_droit=load2(X, i+1,  j0+1);





  // Calcule des mins de la bordure gauche
  uint8 min_bordure_gauche_haut   = min3(b1, b6,  b8);
  uint8 min_bordure_gauche_milieu = min3(b6, b8,  b10);
  uint8 min_bordure_gauche_bas    = min3(b8, b10, b12);

  // Calcule des mins de la colonne gauche
  uint8 min_colonne_gauche_haut   = min3(b2,            haut_gauche,    milieu_gauche );
  uint8 min_colonne_gauche_milieu = min3(haut_gauche,   milieu_gauche,  bas_gauche    );
  uint8 min_colonne_gauche_bas    = min3(milieu_gauche, bas_gauche,     b13           );

  // Calcule des mins de la colonne milieu
  uint8 min_colonne_milieu_haut   = min3(b3,            haut_milieu,    milieu_milieu );
  uint8 min_colonne_milieu_milieu = min3(haut_milieu,   milieu_milieu,  bas_milieu    );
  uint8 min_colonne_milieu_bas    = min3(milieu_milieu, bas_milieu,     b14           );

  // Calcule des mins de la colonne droite
  uint8 min_colonne_droite_haut   = min3(b4,            haut_droit,     milieu_droit  );
  uint8 min_colonne_droite_milieu = min3(haut_droit,    milieu_droit,   bas_droit     );
  uint8 min_colonne_droite_bas    = min3(milieu_droit,  bas_droit,      b15           );

  // Calcul des mins de la bordure de droite
  uint8 min_bordure_droite_haut;
  uint8 min_bordure_droite_milieu;
  uint8 min_bordure_droite_bas;


  // Erosion ===================================================================
  // COLONNE DE GAUCHE =========================================================
  uint8 erosion_haut_gauche   = min3(min_bordure_gauche_haut,   min_colonne_gauche_haut,    min_colonne_milieu_haut);
  uint8 erosion_milieu_gauche = min3(min_bordure_gauche_milieu, min_colonne_gauche_milieu,  min_colonne_milieu_milieu);
  uint8 erosion_bas_gauche    = min3(min_bordure_gauche_bas,    min_colonne_gauche_bas,     min_colonne_milieu_bas);

  // COLONNE DU MILIEU =========================================================
  uint8 erosion_haut_milieu   = min3(min_colonne_gauche_haut,    min_colonne_milieu_haut,   min_colonne_droite_haut);
  uint8 erosion_milieu_milieu = min3(min_colonne_gauche_milieu,  min_colonne_milieu_milieu, min_colonne_droite_milieu);
  uint8 erosion_bas_milieu    = min3(min_colonne_gauche_bas,     min_colonne_milieu_bas,    min_colonne_droite_bas);

  // COLONNE DE DROITE =========================================================
  uint8 erosion_haut_droit ;
  uint8 erosion_milieu_droit ;
  uint8 erosion_bas_droit;


  //Dilatation de la colonne gauche et milieu  =================================
  uint8 dilatation_c1 = max3(erosion_haut_gauche, erosion_milieu_gauche, erosion_bas_gauche);
  uint8 dilatation_c2 = max3(erosion_haut_milieu, erosion_milieu_milieu, erosion_bas_milieu);
  uint8 dilatation_c3, dilatation_res;

  int j_tmp;


  int reste = (j1-j0+1)  % 5;
  for (int j=j0; j<= j1-reste; j+=5){

    //  =====================           J   +   0     =============================
    /*
      Charger la bordure de droite dans les variables de la bordure de droite
      g m d
    */
    //============ LOAD ==================
    b5 =load2(X, i-2, j+2);
    b7 =load2(X, i-1, j+2);
    b9 =load2(X, i,   j+2);
    b11=load2(X, i+1, j+2);
    b16=load2(X, i+2, j+2);

    min_bordure_droite_haut = min3(b5, b7, b9);
    min_bordure_droite_milieu = min3(b7, b9, b11);
    min_bordure_droite_bas = min3(b9, b11, b16);

    erosion_haut_droit    = min3(min_colonne_milieu_haut,    min_colonne_droite_haut,    min_bordure_droite_haut);
    erosion_milieu_droit  = min3(min_colonne_milieu_milieu,  min_colonne_droite_milieu,  min_bordure_droite_milieu);
    erosion_bas_droit     = min3(min_colonne_milieu_bas,     min_colonne_droite_bas,     min_bordure_droite_bas);

    //Dilatation du point i,j ====================================================
    dilatation_c3 = max3(erosion_haut_droit,  erosion_milieu_droit,  erosion_bas_droit);
    dilatation_res = max3(dilatation_c1, dilatation_c2, dilatation_c3);

    //============ Store ==================
    //Charger la valeur dans Y
    store2(Y, i, j, dilatation_res);



    // //  =====================           J   +   1     =============================
    /*
    Charger la bordure de droite de j+1 dans les variables de la bordure de gauhce
    et mettre à jour la colonne de gauche avec  d bd (bg)

    m d g
    */
    j_tmp = j+1;

    b1 =load2(X, i-2, j_tmp+2);
    b6 =load2(X, i-1, j_tmp+2);
    b8 =load2(X, i,   j_tmp+2);
    b10=load2(X, i+1, j_tmp+2);
    b12=load2(X, i+2, j_tmp+2);

    // Calcule des mins de la bordure gauche
    min_bordure_gauche_haut   = min3(b1, b6,  b8);
    min_bordure_gauche_milieu = min3(b6, b8,  b10);
    min_bordure_gauche_bas    = min3(b8, b10, b12);

    erosion_haut_gauche   = min3(min_colonne_droite_haut,   min_bordure_droite_haut,    min_bordure_gauche_haut);
    erosion_milieu_gauche = min3(min_colonne_droite_milieu, min_bordure_droite_milieu,  min_bordure_gauche_milieu);
    erosion_bas_gauche    = min3(min_colonne_droite_bas,    min_bordure_droite_bas,     min_bordure_gauche_bas);

    //Dilatation du point i,j ====================================================
    dilatation_c1 = max3(erosion_haut_gauche, erosion_milieu_gauche, erosion_bas_gauche);
    dilatation_res = max3(dilatation_c1, dilatation_c2, dilatation_c3);

    //============ Store ==================
    //Charger la valeur dans Y
    store2(Y, i, j_tmp, dilatation_res);


    // //  =====================           J   +   2     =============================
    /*
      Charger la bordure de droite de j+2 dans la colonne gauche et mettre à jour
      la colonne milieu avec bd bg (g)
      d g m
    */
    j_tmp = j+2;

    b2            =load2(X, i-2, j_tmp+2);
    haut_gauche   =load2(X, i-1, j_tmp+2);
    milieu_gauche =load2(X, i,   j_tmp+2);
    bas_gauche    =load2(X, i+1, j_tmp+2);
    b13           =load2(X, i+2, j_tmp+2);

    min_colonne_gauche_haut   = min3(b2,            haut_gauche,    milieu_gauche );
    min_colonne_gauche_milieu = min3(haut_gauche,   milieu_gauche,  bas_gauche    );
    min_colonne_gauche_bas    = min3(milieu_gauche, bas_gauche,     b13           );

    erosion_haut_milieu   = min3(min_bordure_droite_haut,    min_bordure_gauche_haut,     min_colonne_gauche_haut);
    erosion_milieu_milieu = min3(min_bordure_droite_milieu,  min_bordure_gauche_milieu,   min_colonne_gauche_milieu);
    erosion_bas_milieu    = min3(min_bordure_droite_bas,     min_bordure_gauche_bas,      min_colonne_gauche_bas);

    //Dilatation du point i,j ====================================================
    dilatation_c2 = max3(erosion_haut_milieu, erosion_milieu_milieu, erosion_bas_milieu);
    dilatation_res = max3(dilatation_c1, dilatation_c2, dilatation_c3);

    //============ Store ==================
    //Charger la valeur dans Y
    store2(Y, i, j_tmp, dilatation_res);



    // //  =====================           J   +   3     =============================
    /*
      Charger la bordure de droite de j+3 dans la colonne milieu et mettre à jour
      la colonne droite avec bg g (m)

      g m d
    */
    j_tmp = j+3;

    b3            =load2(X, i-2, j_tmp+2);
    haut_milieu   =load2(X, i-1, j_tmp+2);
    milieu_milieu =load2(X, i,   j_tmp+2);
    bas_milieu    =load2(X, i+1, j_tmp+2);
    b14           =load2(X, i+2, j_tmp+2);

    min_colonne_milieu_haut   = min3(b3,            haut_milieu,    milieu_milieu );
    min_colonne_milieu_milieu = min3(haut_milieu,   milieu_milieu,  bas_milieu    );
    min_colonne_milieu_bas    = min3(milieu_milieu, bas_milieu,     b14           );

    erosion_haut_droit    = min3(min_bordure_gauche_haut,     min_colonne_gauche_haut,    min_colonne_milieu_haut);
    erosion_milieu_droit  = min3(min_bordure_gauche_milieu,   min_colonne_gauche_milieu,  min_colonne_milieu_milieu);
    erosion_bas_droit     = min3(min_bordure_gauche_bas,      min_colonne_gauche_bas,     min_colonne_milieu_bas);

    //Dilatation du point i,j ====================================================
    dilatation_c3 = max3(erosion_haut_droit,  erosion_milieu_droit,  erosion_bas_droit);
    dilatation_res = max3(dilatation_c1, dilatation_c2, dilatation_c3);

    //============ Store ==================
    //Charger la valeur dans Y
    store2(Y, i, j_tmp, dilatation_res);


    // //  =====================           J   +   4     =============================
    /*
      Charger la bordure de droite de j+4 dans la colonne droite et mettre à jour
      la colonne gauche avec  g m (d)

      m d g
    */
    j_tmp = j+4;
    b4           =load2(X, i-2, j_tmp+2);
    haut_droit   =load2(X, i-1, j_tmp+2);
    milieu_droit =load2(X, i,   j_tmp+2);
    bas_droit    =load2(X, i+1, j_tmp+2);
    b15          =load2(X, i+2, j_tmp+2);

    min_colonne_droite_haut   = min3(b4,            haut_droit,     milieu_droit  );
    min_colonne_droite_milieu = min3(haut_droit,    milieu_droit,   bas_droit     );
    min_colonne_droite_bas    = min3(milieu_droit,  bas_droit,      b15           );

    erosion_haut_gauche   = min3(min_colonne_gauche_haut,    min_colonne_milieu_haut,    min_colonne_droite_haut);
    erosion_milieu_gauche = min3(min_colonne_gauche_milieu,  min_colonne_milieu_milieu,  min_colonne_droite_milieu);
    erosion_bas_gauche    = min3(min_colonne_gauche_bas,     min_colonne_milieu_bas,     min_colonne_droite_bas);

    //Dilatation du point i,j ====================================================
    dilatation_c1 = max3(erosion_haut_gauche, erosion_milieu_gauche, erosion_bas_gauche);
    dilatation_res = max3(dilatation_c1, dilatation_c2, dilatation_c3);

    //============ Store ==================
    //Charger la valeur dans Y
    store2(Y, i, j_tmp, dilatation_res);


    /*
      Charger la bordure de droite de j+4 dans la bordure droite et mettre à jour
      la colonne milieu avec  m d (bd)
      d  g  m

      g  m  d
    */

    dilatation_c2 = dilatation_c1;
    dilatation_c1 = dilatation_c3 ;



  }// fin de la boucle for

  if (reste > 0){
    line_ouverture3_ui8matrix_fusion(X, i, j1-reste+1, j1, Y);
  }

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
  for (int i=i0; i<=i1; i++){
    line_ouverture3_ui8matrix_fusion(X, i, j0, j1, Y);
  }
}
// --------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_ilu5_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------
{
  for (int i=i0; i<=i1; i++){
    line_ouverture3_ui8matrix_fusion_ilu5_red(X, i, j0, j1, Y);
  }
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
