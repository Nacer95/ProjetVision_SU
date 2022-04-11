// -----------------------------------------------------------------------
void line_min3_ui8matrix_ilu3(uint8 **X, int i, int j0, int j1, uint8 **Y)
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
    line_min3_ui8matrix_basic(X, i, j, j_fin, Y);
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

  for (int j=j0; j<=j1; j+=3 ){
    // J
    haut_droit    = load2(X, i-1, j+1);
    milieu_droit  = load2(X, i, j+1);
    bas_droit     = load2(X, i+1, j+1);

    uint8 m1 = min3(haut_droit, haut_milieu, haut_gauche);
    uint8 m2 = min3(milieu_droit, milieu_milieu, milieu_gauche);
    uint8 m3 = min3(bas_gauche, bas_milieu, bas_droit);

    uint8 m = min3(m1, m2, m3);
    store2(Y, i, j, m);

    // J+1
    j_tmp = j+1;

    haut_gauche   = haut_milieu;
    milieu_gauche = milieu_milieu;
    bas_gauche    = bas_milieu;

    haut_milieu   = haut_droit;
    milieu_milieu = milieu_droit;
    bas_milieu    = bas_droit;

    haut_droit    = load2(X, i-1, j_tmp+1);
    milieu_droit  = load2(X, i, j_tmp+1);
    bas_droit     = load2(X, i+1, j_tmp+1);

    m1 = min3(haut_droit, haut_milieu, haut_gauche);
    m2 = min3(milieu_droit, milieu_milieu, milieu_gauche);
    m3 = min3(bas_gauche, bas_milieu, bas_droit);

    m = min3(m1, m2, m3);
    store2(Y, i, j_tmp, m);

    // J+2
    j_tmp = j+2;

    haut_gauche   = haut_milieu;
    milieu_gauche = milieu_milieu;
    bas_gauche    = bas_milieu;

    haut_milieu   = haut_droit;
    milieu_milieu = milieu_droit;
    bas_milieu    = bas_droit;

    haut_droit    = load2(X, i-1, j_tmp+1);
    milieu_droit  = load2(X, i, j_tmp+1);
    bas_droit     = load2(X, i+1, j_tmp+1);

    m1 = min3(haut_droit, haut_milieu, haut_gauche);
    m2 = min3(milieu_droit, milieu_milieu, milieu_gauche);
    m3 = min3(bas_gauche, bas_milieu, bas_droit);

    m = min3(m1, m2, m3);
    store2(Y, i, j_tmp, m);

    // suivant
    haut_gauche   = haut_milieu;
    milieu_gauche = milieu_milieu;
    bas_gauche    = bas_milieu;

    haut_milieu   = haut_droit;
    milieu_milieu = milieu_droit;
    bas_milieu    = bas_droit;

  }



  // ---------------------------------------------------------------------------------------
  void line_min3_ui8matrix_ilu3_elu2_red_factor1(uint8 **X, int i, int j0, int j1, uint8 **Y)
  // ---------------------------------------------------------------------------------------
  {
      uint8 haut_gauche, haut_milieu, haut_droit,
            milieu_gauche, milieu_milieu, milieu_droit,
            bas_gauche, bas_milieu, bas_droit,
            min_all, min_gauche, min_milieu, min_actu,

            gauche_elu , milieu_elu, droite_elu,
            min_gauche_elu, min_milieu_elu, min_droite_elu, min_all_elu,

            min_mg_bg, min_mm_bm, min_md_bd, min_g_m, min_g_m_elu;

      haut_gauche = load2(X, i-1, j0-1);
      milieu_gauche = load2(X, i, j0-1);
      bas_gauche = load2(X, i+1, j0-1);
      gauche_elu = load2(X, i+2, j0-1);

      haut_milieu = load2(X, i-1, j0);
      milieu_milieu = load2(X, i, j0);
      bas_milieu = load2(X, i+1, j0);
      milieu_elu = load2(X, i+2, j0);

      min_mg_bg = min2(milieu_gauche, bas_gauche);

      min_gauche = min2(haut_gauche, min_mg_bg);
      min_gauche_elu = min2(min_mg_bg, gauche_elu);

      min_mm_bm = min2(milieu_milieu, bas_milieu);
      min_milieu = min2(haut_milieu, min_mm_bm);
      min_milieu_elu = min2(min_mm_bm, milieu_elu);

      min_g_m_elu = min2(min_gauche_elu, min_milieu_elu);

      for( int j = j0; j < j1-3; j+=3){
          // ------ 1 ------
          haut_droit = load2(X, i-1, j+1);
          milieu_droit = load2(X, i, j+1);
          bas_droit = load2(X, i+1, j+1);

          droite_elu = load2(X, i+2, j+1);
          /////////////////////////////////////

          min_md_bd = min2(milieu_droit, bas_droit);
          min_g_m = min2(min_gauche, min_milieu);

          min_actu = min2(haut_droit, min_md_bd);
          min_all = min2(min_g_m, min_actu);
          store2(Y, i, j, min_all);

          min_droite_elu = min2(min_md_bd, droite_elu);

          min_all_elu = min2(min_g_m_elu, min_droite_elu);
          store2(Y, i+1, j, min_all_elu);

          min_g_m_elu = min2(min_milieu_elu, min_droite_elu);

          // ------ 2 ------
          haut_droit = load2(X, i-1, j+2);
          milieu_droit = load2(X, i, j+2);
          bas_droit = load2(X, i+1, j+2);

          droite_elu = load2(X, i+2, j+2);

          min_md_bd = min2(milieu_droit, bas_droit);
          min_g_m = min2(min_milieu, min_actu);

          min_gauche = min2(haut_droit, min_md_bd);
          min_all = min2(min_g_m, min_gauche);
          store2(Y, i, j+1, min_all);

          min_gauche_elu = min2(min_md_bd, droite_elu);

          min_all_elu = min2(min_g_m_elu, min_gauche_elu);
          store2(Y, i+1, j+1, min_all_elu);

          min_g_m_elu = min2(min_droite_elu, min_gauche_elu);
          // ------ 3 ------
          haut_droit = load2(X, i-1, j+3);
          milieu_droit = load2(X, i, j+3);
          bas_droit = load2(X, i+1, j+3);

          droite_elu = load2(X, i+2, j+3);

          min_md_bd = min2(milieu_droit, bas_droit);
          min_g_m = min2(min_actu, min_gauche);

          min_milieu = min2(haut_droit, min_md_bd);
          min_all = min2(min_g_m, min_milieu);
          store2(Y, i, j+2, min_all);

          min_milieu_elu = min2(min_md_bd, droite_elu);

          min_all_elu = min2(min_g_m_elu, min_milieu_elu);
          store2(Y, i+1, j+2, min_all_elu);

          min_g_m_elu = min2(min_gauche_elu, min_milieu_elu);
      }

      int r = (j1-j0-1) % 3;
      line_min3_ui8matrix_basic(X, i, j1-r-1, j1, Y);
      line_min3_ui8matrix_basic(X, i+1, j1-r-1, j1, Y);
  }




  // ------------------------------------------------------------------------------------
  void min3_ui8matrix_ilu3_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
  // ------------------------------------------------------------------------------------
  {
    int reste = (i1-i0)%2;
    int i;

    for ( i= i0; i<(i1-reste) ; i+=2){
      line_min3_ui8matrix_ilu3_elu2_red(X, i, j0, j1, Y);
    }
    for (int r=i; r <= i1; r++){ // Je garde cette boucle pour avoir une api générique
      line_min3_ui8matrix_ilu3_elu2_red(X, i, j0, j1, Y );
    }
  }



  // ---------------------------------------------------------------------------
  void line_min3_ui8matrix_ilu3_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
  // ---------------------------------------------------------------------------
  /*
  enregistrer la valeur des colonne sans les recalculer
  */
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

    int j_tmp;
    int j;
    int reste = (j1 - j0 + 1) % 3;

    for (int j=j0; j<=(j1-reste); j+=3 ){
      // J
      haut_droit    = load2(X, i-1, j+1);
      milieu_droit  = load2(X, i, j+1);
      bas_droit     = load2(X, i+1, j+1);

      uint8 m_droit = min3(haut_droit, milieu_droit, bas_droit);
      uint8 m_milieu = min3(haut_milieu, milieu_milieu, bas_milieu);
      uint8 m_gauche = min3(haut_gauche, milieu_gauche, bas_gauche);

      uint8 m = min3(m_gauche, m_milieu, m_droit);
      store2(Y, i, j, m);

      // J+1
      j_tmp = j+1;

      haut_gauche   = load2(X, i-1, j_tmp+1);
      milieu_gauche = load2(X, i, j_tmp+1);
      bas_gauche    = load2(X, i+1, j_tmp+1);

      m_gauche = min3(haut_gauche, milieu_gauche, bas_gauche);

      m = min3(m_gauche, m_milieu, m_droit);
      store2(Y, i, j_tmp, m);

      // J+2
      j_tmp = j+2;

      haut_milieu   = load2(X, i-1, j_tmp+1);
      milieu_milieu = load2(X, i, j_tmp+1);
      bas_milieu    = load2(X, i+1, j_tmp+1);

      m_milieu = min3(haut_milieu, milieu_milieu, bas_milieu);

      m = min3(m_gauche, m_milieu, m_droit);
      store2(Y, i, j_tmp, m);

    }
    for (int r = j ; r<=j1; r++){
      haut_droit    = load2(X, i-1, j+1);
      milieu_droit  = load2(X, i, j+1);
      bas_droit     = load2(X, i+1, j+1);

      uint8 m1 = min3(haut_droit, haut_milieu, haut_gauche);
      uint8 m2 = min3(milieu_droit, milieu_milieu, milieu_gauche);
      uint8 m3 = min3(bas_gauche, bas_milieu, bas_droit);

      uint8 m = min3(m1, m2, m3);
      store2(Y, i, r, m);
    }
  }
//+===================================================================================













//EROSION
// min_Col1 = min5(b1,     b6,             b8,         b10,    b12);
// min_Col2 = min5(b2, haut_gauche, milieu_gauche, bas_gauche, b13);
// min_Col3 = min5(b3, haut_milieu, milieu_milieu, bas_milieu, b14);
// min_Col4 = min5(b4, haut_droit,  milieu_droit,  bas_droit,  b15);
// min_Col5 = min5(b5,     b7,             b9,         b11,    b16);
//
// min_res = min5(min_Col1, min_Col2, min_Col3, min_Col4, min_Col5 );












void line_ouverture3_ui8matrix_fusion_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
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

  uint8 min_Col1, min_Col2, min_Col3, min_Col4, min_Col5;
  uint8 min_res;


  b1 =load2(X, i-2, j0-2);      b2 =load2(X, i-2, j0-1);     b3 =load2(X, i-2, j0);    b4 =load2(X, i-2, j0+1);
  b6 =load2(X, i-1, j0-2);
  b8 =load2(X, i,   j0-2);
  b10=load2(X, i+1, j0-2);
  b12=load2(X, i+2, j0-2);      b13=load2(X, i+2, j0-1);     b14=load2(X, i+2, j0);    b15=load2(X, i+2, j0+1);

                      haut_gauche  =load2(X, i-1, j0-1);    haut_milieu=load2(X, i-1, j0);      haut_droit=load2(X, i-1, j0+1);
                      milieu_gauche=load2(X, i, j0-1);      milieu_milieu=load2(X, i, j0);      milieu_droit=load2(X, i, j0+1);
                      bas_gauche   =load2(X, i+1, j0-1);    bas_milieu=load2(X, i+1, j0);       bas_droit=load2(X, i+1,  j0+1);


  // COLONNE DE GAUCHE =========================================================
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


  // COLONNE DU MILIEU =========================================================
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


  //Dilatation du point i,j
  uint8 dilatation_c1 = max3(erosion_haut_gauche, erosion_milieu_gauche, erosion_bas_gauche);
  uint8 dilatation_c2 = max3(erosion_haut_milieu, erosion_milieu_milieu, erosion_bas_milieu);
  uint8 dilatation_c3, dilatation_res;

  // COLONNE DROITE A VENIR ====================================================
  // MAJ haut_droit
  uint8 haut_droit_c1, haut_droit_c2 , haut_droit_c3, erosion_haut_droit;

  // MAJ milieu_droit
  uint8 milieu_droit_c1, milieu_droit_c2,  milieu_droit_c3, erosion_milieu_droit;

  // MAJ bas_droit
  uint8 bas_droit_c1, bas_droit_c2, bas_droit_c3, erosion_bas_droit;



  for (int j=j0; j<= j1; j++){

    //============ LOAD ==================
    b5 =load2(X, i-2, j+2);
    b7 =load2(X, i-1, j+2);
    b9 =load2(X, i,   j+2);
    b11=load2(X, i+1, j+2);
    b16=load2(X, i+2, j+2);


    // COLONNE DROITE ==========================================================
    // MAJ haut_droit
    haut_droit_c1 = min3(b3, haut_milieu, milieu_milieu);
    haut_droit_c2 = min3(b4, haut_droit,  milieu_droit);
    haut_droit_c3 = min3(b5, b7, b9);
    erosion_haut_droit = min3(haut_droit_c1, haut_droit_c2, haut_droit_c3);

    // MAJ milieu_droit
    milieu_droit_c1 = min3(haut_milieu, milieu_milieu, bas_milieu);
    milieu_droit_c2 = min3(haut_droit,  milieu_droit,  bas_droit);
    milieu_droit_c3 = min3(b7, b9, b11);
    erosion_milieu_droit = min3(milieu_droit_c1, milieu_droit_c2, milieu_droit_c3);

    // MAJ bas_droit
    bas_droit_c1 = min3(milieu_milieu, bas_milieu, b14);
    bas_droit_c2 = min3(milieu_droit,  bas_droit,  b15);
    bas_droit_c3 = min3(b9, b11, b16);
    erosion_bas_droit = min3(bas_droit_c1, bas_droit_c2, bas_droit_c3);


    //Dilatation du point i,j
    dilatation_c3 = max3(erosion_haut_droit,  erosion_milieu_droit,  erosion_bas_droit);
    dilatation_res = max3(dilatation_c1, dilatation_c2, dilatation_c3);

    //============ Store ==================
    //Charger la valeur dans Y
    store2(Y, i, j, dilatation_res);


    //========== permutation =============
    //==========   EROSION   =============
    //bordure gauche <- gauche
    b1 = b2;
    b6 = haut_gauche;
    b8 = milieu_gauche;
    b10 = bas_gauche;
    b12 = b13;
    //gauche <- milieu
    b2 = b3;
    haut_gauche = haut_milieu;
    milieu_gauche = milieu_milieu;
    bas_gauche = bas_milieu;
    b13 = b14;

    // milieu <- droite
    b3 = b4;
    haut_milieu = haut_droit;
    milieu_milieu = milieu_droit;
    bas_milieu = bas_droit;
    b14 = b15;

    // droite <- bordure droite
    b4 = b5;
    haut_droit = b7;
    milieu_droit = b9;
    bas_milieu = b11;
    b15 = b16;

    //==========  Dilatation =============
    // gauche <- milieu
    erosion_haut_gauche   = erosion_haut_milieu ;
    erosion_milieu_gauche = erosion_milieu_milieu;
    erosion_bas_gauche    = erosion_bas_milieu;

    // milieu <- droit
    erosion_haut_milieu   = erosion_haut_droit;
    erosion_milieu_milieu = erosion_milieu_droit;
    erosion_bas_milieu    = erosion_bas_droit;

    //dilatation
    dilatation_c1 = dilatation_c2;
    dilatation_c2 = dilatation_c3;

  }// fin de la boucle for





















  // ----------------------------------------------------------------------------------------
  void line_ouverture3_ui8matrix_fusion_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
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


    // COLONNE DE GAUCHE =========================================================
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


    // COLONNE DU MILIEU =========================================================
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


    //Dilatation du point i,j ====================================================
    uint8 dilatation_c1 = max3(erosion_haut_gauche, erosion_milieu_gauche, erosion_bas_gauche);
    uint8 dilatation_c2 = max3(erosion_haut_milieu, erosion_milieu_milieu, erosion_bas_milieu);
    uint8 dilatation_c3, dilatation_res;

    // COLONNE DROITE A VENIR ====================================================
    // MAJ haut_droit
    uint8 haut_droit_c1, haut_droit_c2 , haut_droit_c3, erosion_haut_droit;

    // MAJ milieu_droit
    uint8 milieu_droit_c1, milieu_droit_c2,  milieu_droit_c3, erosion_milieu_droit;

    // MAJ bas_droit
    uint8 bas_droit_c1, bas_droit_c2, bas_droit_c3, erosion_bas_droit;

    int j_tmp;

    int reste = (j1-j0)  % 5;
    for (int j=j0; j<= j1; j+=1){

      //  =====================           J   +   0     =============================
      /*
        Charger la bordure de droite dans les variables de la bordure de droite
      */
      //============ LOAD ==================
      b5 =load2(X, i-2, j+2);
      b7 =load2(X, i-1, j+2);
      b9 =load2(X, i,   j+2);
      b11=load2(X, i+1, j+2);
      b16=load2(X, i+2, j+2);

      // COLONNE DROITE ==========================================================
      // MAJ haut_droit
      haut_droit_c1 = min3(b3, haut_milieu, milieu_milieu);
      haut_droit_c2 = min3(b4, haut_droit,  milieu_droit);
      haut_droit_c3 = min3(b5, b7, b9);
      erosion_haut_droit = min3(haut_droit_c1, haut_droit_c2, haut_droit_c3);

      // MAJ milieu_droit
      milieu_droit_c1 = min3(haut_milieu, milieu_milieu, bas_milieu);
      milieu_droit_c2 = min3(haut_droit,  milieu_droit,  bas_droit);
      milieu_droit_c3 = min3(b7, b9, b11);
      erosion_milieu_droit = min3(milieu_droit_c1, milieu_droit_c2, milieu_droit_c3);

      // MAJ bas_droit
      bas_droit_c1 = min3(milieu_milieu, bas_milieu, b14);
      bas_droit_c2 = min3(milieu_droit,  bas_droit,  b15);
      bas_droit_c3 = min3(b9, b11, b16);
      erosion_bas_droit = min3(bas_droit_c1, bas_droit_c2, bas_droit_c3);

      //Dilatation du point i,j ====================================================
      dilatation_c3 = max3(erosion_haut_droit,  erosion_milieu_droit,  erosion_bas_droit);
      dilatation_res = max3(dilatation_c1, dilatation_c2, dilatation_c3);

      //============ Store ==================
      //Charger la valeur dans Y
      store2(Y, i, j, dilatation_res);



      //========== permutation =============
      //==========   EROSION   =============
      // bordure gauche <- gauche
      b1 = b2;
      b6 = haut_gauche;
      b8 = milieu_gauche;
      b10 = bas_gauche;
      b12 = b13;

      // gauche <- milieu
      b2 = b3;
      haut_gauche = haut_milieu;
      milieu_gauche = milieu_milieu;
      bas_gauche = bas_milieu;
      b13 = b14;

      //milieu <- droit
      b3 = b4;
      haut_milieu = haut_droit;
      milieu_milieu = milieu_droit;
      bas_milieu = bas_droit;
      b14 = b15;

      // droit <- bordure d
      b4 = b5;
      haut_droit=b7;
      milieu_droit=b9;
      bas_droit=b11;
      b15=b16;

      // //==========  Dilatation =============
      // COLONNE DE GAUCHE =========================================================
      // gauche <- milieu
       erosion_haut_gauche = erosion_haut_milieu;
       erosion_milieu_gauche = erosion_milieu_milieu;
       erosion_bas_gauche = erosion_bas_milieu;

      // COLONNE DU MILIEU =========================================================
      // milieu <- droit
       erosion_haut_milieu = erosion_haut_droit;
       erosion_milieu_milieu = erosion_milieu_droit;
       erosion_bas_milieu = erosion_bas_droit;


      //Dilatation du point i,j ====================================================
       dilatation_c1 = dilatation_c2;
       dilatation_c2 = dilatation_c3;


    }// fin de la boucle for

    // if (reste >= 0){
    //   line_ouverture3_ui8matrix_fusion(X, i, j1-reste+1, j1, Y);
    // }

  }











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

        g  m  d <- objectif
        milieu <- gauche
        gauche <- droit
        droit  <- calculé au tour suivant
      */

      dilatation_c2 = dilatation_c1;
      dilatation_c1 = dilatation_c3 ;

    }// fin de la boucle for

    if (reste > 0){
      line_ouverture3_ui8matrix_fusion(X, i, j1-reste+1, j1, Y);
    }

  }


  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
  //======================  //======================  //======================  //======================  //======================  //======================  //======================  //======================
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
   printf("moi\n");
   // pas d'epilogue car utilisation du padding
   for (int j = j0; j<= j1; j++){
     //charger la bordure de gauche
     uint8 haut_gauche   = load2(T, i-1 , j-1 );
     uint8 milieu_gauche = load2(T, i   , j-1 );
     uint8 bas_gauche    = load2(T, i-1 , j-1 );

     uint8 haut_milieu   = load2(T, i-1 , j );
     uint8 milieu_milieu = load2(T, i   , j );
     uint8 bas_milieu    = load2(T, i+1 , j );

     uint8 haut_droit    = load2(T, i-1,   j+1);
     uint8 milieu_droit  = load2(T, i  ,   j+1);
     uint8 bas_droit     = load2(T, i+1,   j+1);


     uint8 min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
     uint8 min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);
     uint8 min3_droite  = mot_min3(haut_droit,  milieu_droit,  bas_droit);

     //permutation
     //Le Dernier de droite et les dernier de b
     uint8 mot_pemutation_gauche = macro_ui8left1(min3_gauche,  min3_milieu);
     uint8 mot_permutation_droit = macro_ui8right1(min3_milieu, min3_droite);

     //min
     uint8 min3_res = mot_min3(mot_pemutation_gauche, min3_milieu, mot_permutation_droit);

     //store
     store2(Z, i, j, min3_res);

     //permutation colonne gauche
     // haut_gauche   = haut_milieu;
     // milieu_gauche = milieu_milieu;
     // bas_gauche    = bas_milieu;
     //
     // haut_milieu   = haut_droit;
     // milieu_milieu = milieu_droit;
     // bas_milieu    = bas_droit;

   }
  }

  //   //charger la bordure de gauche
  // uint8 haut_gauche = 0;
  // uint8 milieu_gauche = 0;
  // uint8 bas_gauche = 0;
  // uint8 haut_milieu   = load2(X8, i-1 , j0);
  // uint8 milieu_milieu = load2(X8, i   , j0);
  // uint8 bas_milieu    = load2(X8, i+1 , j0);
  // uint8 min3_gauche  = mot_max3(haut_gauche, milieu_gauche, bas_gauche);
  // uint8 min3_milieu  = mot_max3(haut_milieu, milieu_milieu, bas_milieu);
  // uint8 haut_droit,  milieu_droit,  bas_droit, min3_droite;
  // uint8 mot_pemutation_gauche , mot_permutation_droit;
  // uint8 min3_res;
  // // pas d'epilogue car utilisation du padding
  // // printf("Ligne : %d\n", i);
  // for (int j = j0; j<= j1; j++){
  //     haut_droit    = load2(X8, i-1,   j+1);
  //     milieu_droit  = load2(X8, i  ,   j+1);
  //     bas_droit     = load2(X8, i+1,   j+1);
  //     // printf("M1 : %d M2 : %d M3 : %d\n", haut_gauche, haut_milieu, haut_droit);
  //     // printf("M4 : %d M5 : %d M6 : %d\n", milieu_gauche, milieu_milieu, milieu_droit);
  //     // printf("M8 : %d M9 : %d M10 : %d\n", bas_gauche, bas_milieu, bas_droit);
  //     min3_gauche  = mot_max3(haut_gauche, milieu_gauche, bas_gauche);
  //     min3_milieu  = mot_max3(haut_milieu, milieu_milieu, bas_milieu);
  //     min3_droite  = mot_max3(haut_droit,  milieu_droit,  bas_droit);
  //     // printf("Min g : %d\n", min3_gauche);
  //     // printf("Min m : %d\n", min3_milieu);
  //     // printf("Min d : %d\n", min3_droite);
  //     //permutation
  //     mot_pemutation_gauche = macro_ui8left1(min3_gauche, min3_milieu);
  //     mot_permutation_droit = macro_ui8right1(min3_milieu, min3_droite);
  //     //min
  //     min3_res = mot_max3(mot_pemutation_gauche, min3_milieu, mot_permutation_droit);
  //     // printf("j : %d, r : %d\n", j , min3_res);
  //     //store
  //     store2(Y8, i, j, min3_res);
  //     //permutation colonne gauche
  //     haut_gauche   = haut_milieu;
  //     milieu_gauche = milieu_milieu;
  //     bas_gauche    = bas_milieu;
  //     haut_milieu   = haut_droit;
  //     milieu_milieu = milieu_droit;
  //     bas_milieu    = bas_droit;
  //   }
  // }
  //
  // void line_swp8_ui8matrix_basic_min(uint8 **X8, int i, int j0, int j1, uint8 **Y8)
  // // ------------------------------------------------------------------------
  // {
  //
  //   // // pas d'epilogue car utilisation du padding
  //   // for (int j = j0; j<= j1; j++){
  //   //   //charger la bordure de gauche
  //   //   uint8 haut_gauche   = load2(T, i-1 , j-1 );
  //   //   uint8 milieu_gauche = load2(T, i   , j-1 );
  //   //   uint8 bas_gauche    = load2(T, i-1 , j-1 );
  //   //
  //   //   uint8 haut_milieu   = load2(T, i-1 , j );
  //   //   uint8 milieu_milieu = load2(T, i   , j );
  //   //   uint8 bas_milieu    = load2(T, i+1 , j );
  //   //
  //   //   uint8 haut_droit    = load2(T, i-1,   j+1);
  //   //   uint8 milieu_droit  = load2(T, i  ,   j+1);
  //   //   uint8 bas_droit     = load2(T, i+1,   j+1);
  //   //
  //   //
  //   //   uint8 min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
  //   //   uint8 min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);
  //   //   uint8 min3_droite  = mot_min3(haut_droit,  milieu_droit,  bas_droit);
  //   //
  //   //   //permutation
  //   //   //Le Dernier de droite et les dernier de b
  //   //   uint8 mot_pemutation_gauche = macro_ui8left1(min3_gauche,  min3_milieu);
  //   //   uint8 mot_permutation_droit = macro_ui8right1(min3_milieu, min3_droite);
  //   //
  //   //   //min
  //   //   uint8 min3_res = mot_min3(mot_pemutation_gauche, min3_milieu, mot_permutation_droit);
  //   //
  //   //   //store
  //   //   store2(Z, i, j, min3_res);
  //   //
  //   //   //permutation colonne gauche
  //   //   // haut_gauche   = haut_milieu;
  //   //   // milieu_gauche = milieu_milieu;
  //   //   // bas_gauche    = bas_milieu;
  //   //   //
  //   //   // haut_milieu   = haut_droit;
  //   //   // milieu_milieu = milieu_droit;
  //   //   // bas_milieu    = bas_droit;
  //   //
  //   // }
  //
  //   //charger la bordure de gauche
  // uint8 haut_gauche = 0;
  // uint8 milieu_gauche = 0;
  // uint8 bas_gauche = 0;
  // uint8 haut_milieu   = load2(X8, i-1 , j0);
  // uint8 milieu_milieu = load2(X8, i   , j0);
  // uint8 bas_milieu    = load2(X8, i+1 , j0);
  // uint8 min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
  // uint8 min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);
  // uint8 haut_droit,  milieu_droit,  bas_droit, min3_droite;
  // uint8 mot_pemutation_gauche , mot_permutation_droit;
  // uint8 min3_res;
  // // pas d'epilogue car utilisation du padding
  // // printf("Ligne : %d\n", i);
  // for (int j = j0; j<= j1; j++){
  //     haut_droit    = load2(X8, i-1,   j+1);
  //     milieu_droit  = load2(X8, i  ,   j+1);
  //     bas_droit     = load2(X8, i+1,   j+1);
  //     // printf("M1 : %d M2 : %d M3 : %d\n", haut_gauche, haut_milieu, haut_droit);
  //     // printf("M4 : %d M5 : %d M6 : %d\n", milieu_gauche, milieu_milieu, milieu_droit);
  //     // printf("M8 : %d M9 : %d M10 : %d\n", bas_gauche, bas_milieu, bas_droit);
  //     min3_gauche  = mot_min3(haut_gauche, milieu_gauche, bas_gauche);
  //     min3_milieu  = mot_min3(haut_milieu, milieu_milieu, bas_milieu);
  //     min3_droite  = mot_min3(haut_droit,  milieu_droit,  bas_droit);
  //     // printf("Min g : %d\n", min3_gauche);
  //     // printf("Min m : %d\n", min3_milieu);
  //     // printf("Min d : %d\n", min3_droite);
  //     //permutation
  //     mot_pemutation_gauche = macro_ui8left1(min3_gauche, min3_milieu);
  //     mot_permutation_droit = macro_ui8right1(min3_milieu, min3_droite);
  //     //min
  //     min3_res = mot_min3(mot_pemutation_gauche, min3_milieu, mot_permutation_droit);
  //     // printf("j : %d, r : %d\n", j , min3_res);
  //     //store
  //     store2(Y8, i, j, min3_res);
  //     //permutation colonne gauche
  //     haut_gauche   = haut_milieu;
  //     milieu_gauche = milieu_milieu;
  //     bas_gauche    = bas_milieu;
  //     haut_milieu   = haut_droit;
  //     milieu_milieu = milieu_droit;
  //     bas_milieu    = bas_droit;
  //   }
  // }
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


  // void swp8_ui8matrix_basic(uint8 **X, int wX, int hX,  int i0, int i1, int j0, int j1,int w8, uint8 **T8, uint8 **Z8, uint8 **Y)
  // // ----------------------------------------------------------------------------
  // {
  //   // faire le pack de X dans T
  //   pack_ui8matrix(X, hX, wX, T8);
  //   // display_ui8matrix()
  //   // calcule de chaque ligne
  //   for(int i = i0; i<=i1; i++){
  //     line_swp8_ui8matrix_basic(T8, i, j0, j1, Z8);
  //   }
  //   // unpack z dans Y
  //   unpack_ui8matrix (Z8 , hX, w8, Y);
  // }

  // ------------------------------------------------------------------------
  void line_max3_ui8matrix_swp_basic(uint8 **X8, int i, int j0, int j1, uint8 **Y8)
  // ------------------------------------------------------------------------
  {
     //charger la bordure de gauche
     uint8 haut_gauche = 0;
     uint8 milieu_gauche = 0;
     uint8 bas_gauche = 0;

     uint8 haut_milieu   = load2(X8, i-1 , j0);
     uint8 milieu_milieu = load2(X8, i   , j0);
     uint8 bas_milieu    = load2(X8, i+1 , j0);

     uint8 min3_gauche  = mot_max3(haut_gauche, milieu_gauche, bas_gauche);
     uint8 min3_milieu  = mot_max3(haut_milieu, milieu_milieu, bas_milieu);

     uint8 haut_droit,  milieu_droit,  bas_droit, min3_droite;
     uint8 mot_pemutation_gauche , mot_permutation_droit;
     uint8 min3_res;
     // pas d'epilogue car utilisation du padding
     // printf("Ligne : %d\n", i);
     for (int j = j0; j<= j1; j++){
         haut_droit    = load2(X8, i-1,   j+1);
         milieu_droit  = load2(X8, i  ,   j+1);
         bas_droit     = load2(X8, i+1,   j+1);
         // printf("M1 : %d M2 : %d M3 : %d\n", haut_gauche, haut_milieu, haut_droit);
         // printf("M4 : %d M5 : %d M6 : %d\n", milieu_gauche, milieu_milieu, milieu_droit);
         // printf("M8 : %d M9 : %d M10 : %d\n", bas_gauche, bas_milieu, bas_droit);
         min3_gauche  = mot_max3(haut_gauche, milieu_gauche, bas_gauche);
         min3_milieu  = mot_max3(haut_milieu, milieu_milieu, bas_milieu);
         min3_droite  = mot_max3(haut_droit,  milieu_droit,  bas_droit);
         // printf("Min g : %d\n", min3_gauche);
         // printf("Min m : %d\n", min3_milieu);
         // printf("Min d : %d\n", min3_droite);
         //permutation
         mot_pemutation_gauche = macro_ui8left1(min3_milieu, min3_gauche);
         mot_permutation_droit = macro_ui8right1(min3_milieu, min3_droite);
         //min
         min3_res = mot_max3(mot_pemutation_gauche, min3_milieu, mot_permutation_droit);
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
  // -------------------------------------------------------------------------------
  void max3_ui8matrix_swp_basic(uint8 **X8, int h, int w, int i0, int i1, int j0, int j1, uint8 **Y)
  // -------------------------------------------------------------------------------
  {
     uint8 **Y8 =  ui8matrix(0, h, 0, j1+1);
     zero_ui8matrix(Y8,      0, h, 0, j1+1);
     displayR_ui8matrix (X8, i0, i1, j0, j1, "X8_mat");
     for(int i = i0; i<=i1; i++){
       line_max3_ui8matrix_swp_basic(X8, i, j0, j1, Y8);
     }
     // unpack z dans Y
     displayR_ui8matrix (Y8, i0, i1, j0, j1, "Y8_mat");
     display_ui8matrix (Y8,  0, h-1, 0, j1, "%5d", "Y8");
     unpack_ui8matrix (Y8 , h, j1+1, Y);
  }

  // // -------------------------------------------------------------------------------
  // void min3_ui8matrix_swp_basic(uint8 **X8, int h, int w, int i0, int i1, int j0, int j1, uint8 **Y)
  // // -------------------------------------------------------------------------------
  // {
  //     uint8 **Y8 =  ui8matrix(0, h, 0, j1+1);
  //     zero_ui8matrix(Y8,      0, h, 0, j1+1);
  //     // displayR_ui8matrix (X8, i0, i1, j0, j1, "X8_mat");
  //     for(int i = i0; i<=i1; i++){
  //       line_swp8_ui8matrix_basic_min(X8, i, j0, j1, Y8);
  //     }
  //     // unpack z dans Y
  //     displayR_ui8matrix (Y8, i0, i1, j0, j1, "Y8_mat");
  //     // display_ui8matrix (Y8,  0, h-1, 0, j1, "%5d", "Y8");
  //     unpack_ui8matrix (Y8 , h, j1+1, Y);
  // }

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
