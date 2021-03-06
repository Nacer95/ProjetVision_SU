/* --------------- */
/* --- nrdef.h --- */
/* --------------- */

/*
 * Copyright (c) 2000 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique
 */

#ifndef __NRDEF_H__
#define __NRDEF_H__

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define load1(X, i) X[i]
#define load2(X, i, j) X[i][j]
#define store1(X, i, x) X[i] = x
#define store2(X, i, j, x) X[i][j] = x

#define min5(x, y, z, t, h) (x&&y&&z&&t&&h)
#define min3(x, y, z) (x&&y&&z)
#define min2(x, y) (x&&y)



#define max5(x, y, z, t, h) (x||y||z||t||h)
#define max3(x, y, z) (x||y||z)
#define max2(x, y) (x||y)



#define TRUE 1
#define FALSE 0

#define IMAGE_EXPORT(X) X

#endif // __NRDEF_H__
