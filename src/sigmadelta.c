/* -------------------- */
/* --- sigmadelta.c --- */
/* -------------------- */

/*
* Copyright (c) 2004 - 2021, Lionel Lacassagne, All rights reserved
* University of Paris Sud, Laboratoire de Recherche en Informatique
* Sorbonne University - Laboratoire d'Informatique de Paris 6
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"

#include "sigmadelta.h"


// -----------------------------------------------------------------------------------------
void SigmaDelta_Step0_line(uint8 *I, uint8 *M, uint8 *O, uint8 *V, uint8 *E, int j0, int j1)
// -----------------------------------------------------------------------------------------
/*
  *X : X est un tableau une ligne à traiter
  *I : Une ligne de l'image source
  *M : Une ligne de Moyenne de niveau de gris d'un pixel, prend en concidération le bruit
  *O : La différence entre l'image courante et l'image de fond
  *V : Ecart type pour une ligne de pixel
  *E : Etiquette d'une ligne de pixel
  j0 : indice du premier pixel
  j1 : indice du dernier pixel
*/
{
  //Initialiser la moyenne de gris de chaque pixel   M0(x) = I0(x) et V0(x) = Vmin
  //Vmin et Vmax sont fournis par le fichier sigmadelta.h --> inclusion d'un fichier d'un autre dossier? Makefile?
  //Ni Prologue Ni Epilogue incrementation de 1 traitement de tous les pixels
  for (int j=j0; j<=j1 ; j++){
    M[j] = I[j];
    V[j] = SD_VMIN; //Quel est l'impact de la variance sur le résultat final
  }


}
// ------------------------------------------------------------------------------------------------
void SigmaDelta_1Step_line(uint8 *I, uint8 *M, uint8 *O, uint8 *V, uint8 *E, int k, int j0, int j1)
// ------------------------------------------------------------------------------------------------
/*
  TRAVAIL SUR UNE LIGNE
  Mettre à jours la moyenne et la variance de chaque pixel d'une ligne
  Determiner si un pixel est en mouvement ou pas ici E(X) = {0, 1}
*/
{
  //suivre l'algorithme présenté par le sujet
  // step #1 : Mt estimation
  for(int j=j0; j<=j1; j++){
    //comparer la valeur courante d'un pixel avec sa moyenne à t-1 puis mettre à jour la moyenne
    if (M[j] < I[j] ){ M[j]+= 1; }
    if (M[j] > I[j] ){ M[j]-= 1; }
    else { M[j] = M[j] ; } // ici le else est inutile, je le garde pour expliciter le fonctionnement de l'algo
  }
  // step #2 : Ot computation
  for(int j=j0; j<=j1; j++){
    //Enregistrer la différence entre l'image courante et l'image de fond
    //Ot(x) = |Mt(x) − It(x)|
    O[j] = abs(M[j] - I[j]);
  }
  // step #3 : Vt update and clamping
  for(int j=j0; j<=j1; j++){
    // Comparer la variance à k fois la valeur de la différence et mettre à jour la variance (écart type)
    if ( V[j] < (k*O[j]) )  {  V[j]+= 1;  }
    if ( V[j] > (k*O[j]) )  {  V[j]-= 1;  }
    else {  V[j] = V[j]; } // ici le else est inutile, je le garde pour expliciter le fonctionnement de l'algo

    V[j] = MAX(  MIN(V[j], SD_VMAX),   SD_VMIN);
  }
  // step #4 : Et estimation
  for(int j=j0; j<=j1; j++){
    // aprés la mise à jour de la variance (écart type)
    if (O[j] < V[j] )  {  E[j] = 0;  }
    else {  E[j] = 1;  }
  }

}
// ---------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void SigmaDelta_1Step_line_oneFor(uint8 *I, uint8 *M, uint8 *O, uint8 *V, uint8 *E, int k, int j0, int j1)
// ------------------------------------------------------------------------------------------------
/*
  TRAVAIL SUR UNE LIGNE
  Mettre à jours la moyenne et la variance de chaque pixel d'une ligne
  Determiner si un pixel est en mouvement ou pas ici E(X) = {0, 1}
*/
{
  for(int j=j0; j<=j1; j++){

    if (M[j] < I[j] ){ M[j]+= 1; }
    if (M[j] > I[j] ){ M[j]-= 1; }
    else { M[j] = M[j] ; }

    O[j] = abs(M[j] - I[j]);

    if ( V[j] < (k*O[j]) )  {  V[j]+= 1;  }
    if ( V[j] > (k*O[j]) )  {  V[j]-= 1;  }
    else {  V[j] = V[j]; }

    V[j] = MAX(  MIN(V[j], SD_VMAX),   SD_VMIN);

    if (O[j] < V[j] )  {  E[j] = 0;  }
    else {  E[j] = 1;  }
  }

}
// ------------------------------------------------------------------------------------------------
void SigmaDelta_1Step_line_oneFor_reg(uint8 *I, uint8 *M, uint8 *O, uint8 *V, uint8 *E, int k, int j0, int j1)
// ------------------------------------------------------------------------------------------------
/*
  TRAVAIL SUR UNE LIGNE
  Mettre à jours la moyenne et la variance de chaque pixel d'une ligne
  Determiner si un pixel est en mouvement ou pas ici E(X) = {0, 1}
*/
{
  for(int j=j0; j<=j1; j++){
    uint8 ij = I[j];
    uint8 mj = M[j];
    uint8 oj = O[j];
    uint8 vj = V[j];


    double k_oj = k*oj;

    if (mj < ij ){ mj+= 1; }
    if (mj > ij ){ mj-= 1; }
    else { mj = mj ; }


    oj = abs(mj - ij);

    if ( vj < (k_oj) )  {  vj+= 1;  }
    if ( vj > (k_oj) )  {  vj-= 1;  }
    else {  vj = vj; }

    vj = MAX(  MIN(vj, SD_VMAX),   SD_VMIN);

    if (oj < vj )  {  E[j] = 0;  }
    else {  E[j] = 1;  }

    I[j] = ij;
    M[j] = mj;
    O[j] = oj;
    V[j] = vj;


  }

}
void SigmaDelta_Step0(uint8 **I, uint8 **M, uint8 **O, uint8 **V, uint8 **E, int i0, int i1, int j0, int j1)
// ---------------------------------------------------------------------------------------------------------
{
  /*
    **X : X est un tableau de tableau = matrice
    **I : Tous les pixel de l'image source
    **M : Les moyenne de niveau de gris de chaqun des pixels, prise en considération du bruit
    **O : La différence entre l'image courante et l'image de fond
    **V : Ecart type calculer pour chaque pixels
    **E : Etiquette d'une ligne de pixel
    i0  : indice de la primière ligne
    i1  : indice de la dernière ligne
    j0  : indice du premier pixel
    j1  : indice du dernier pixel
  */

  //Initialiser la moyenne et la variance pour chaque pixels de l'image
  for (int i=i0; i<=i0; i++){
    //SigmaDelta_Step0_line initialise les lignes une à une
    SigmaDelta_Step0_line(I[i], M[i], O[i], V[i], E[i], j0, j1);
  }
}
// ----------------------------------------------------------------------------------------------------------------
void SigmaDelta_1Step(uint8 **I, uint8 **M, uint8 **O, uint8 **V, uint8 **E, int k, int i0, int i1, int j0, int j1)
// ----------------------------------------------------------------------------------------------------------------
/*
  TRAVAIL SUR TOUTE UNE IMAGE = MATRICE
  Condition pour appeler la fonction SigmaDelta_1Step:
    - La fonction SigmaDelta_Step0 est appelée avant
    - La matrice I à subit un changement
*/
{
  // L'image est traitée ligne par ligne
  for (int i=i0; i<=i1; i++){
    //fournir les paramètres de la ligne a traitée
    SigmaDelta_1Step_line(I[i], M[i], O[i], V[i], E[i], k, j0, j1);
  }
}
// ----------------------------------------------------------------------------------------------------------------
void SigmaDelta_1Step_oneFor(uint8 **I, uint8 **M, uint8 **O, uint8 **V, uint8 **E, int k, int i0, int i1, int j0, int j1)
// ----------------------------------------------------------------------------------------------------------------
/*
  TRAVAIL SUR TOUTE UNE IMAGE = MATRICE
  Condition pour appeler la fonction SigmaDelta_1Step:
    - La fonction SigmaDelta_Step0 est appelée avant
    - La matrice I à subit un changement
*/
{
  // L'image est traitée ligne par ligne
  for (int i=i0; i<=i1; i++){
    //fournir les paramètres de la ligne a traitée
    SigmaDelta_1Step_line_oneFor(I[i], M[i], O[i], V[i], E[i], k, j0, j1);
  }
}

// ----------------------------------------------------------------------------------------------------------------
void SigmaDelta_1Step_oneFor_reg(uint8 **I, uint8 **M, uint8 **O, uint8 **V, uint8 **E, int k, int i0, int i1, int j0, int j1)
// ----------------------------------------------------------------------------------------------------------------
/*
  TRAVAIL SUR TOUTE UNE IMAGE = MATRICE
  Condition pour appeler la fonction SigmaDelta_1Step:
    - La fonction SigmaDelta_Step0 est appelée avant
    - La matrice I à subit un changement
*/
{
  // L'image est traitée ligne par ligne
  for (int i=i0; i<=i1; i++){
    //fournir les paramètres de la ligne a traitée
    SigmaDelta_1Step_line_oneFor_reg(I[i], M[i], O[i], V[i], E[i], k, j0, j1);
  }
}
