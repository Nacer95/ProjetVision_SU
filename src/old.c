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
