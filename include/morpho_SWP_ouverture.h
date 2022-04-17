/* -------------------------- */
/* --- morpho_SWP_ouverture.h --- */
/* -------------------------- */

/*
 * Copyright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique
 */

#ifndef __MORPHO_SWP_OUVERTURE_H__
#define __MORPHO_SWP_OUVERTURE_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif


// ========================
// == fonction completes ==
// ========================

// pipeline d'operateurs
void ouverture3_ui8matrix_SWP_basic(uint8 **X, int h, int w, int i0, int i1, int j0, int j1, uint8 **Z, uint8 **Y);
void ouverture3_ui8matrix_pipeline_SWP_rotation_trivial(uint8 **X8, int h, int w, int i0, int i1, int j0, int j1, uint8 **T8, uint8 **Y8);


#ifdef __cplusplus
}
#endif

#endif // __MORPHO_OUVERTURE_H__
