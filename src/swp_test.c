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
