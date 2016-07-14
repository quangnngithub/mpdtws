
// This is the primary include file for PDS.  Applications should
// just need to include this file and everything else will
// be taken care of


#ifndef _PDS_H
#define _PDS_H

// Useful macros
#define PDS_MIN(X,Y)   ((X) < (Y) ?  (X) : (Y))
#define PDS_MAX(X,Y)   ((X) < (Y) ?  (Y) : (X))
#define PDS_ABS(a)     (((a) < 0) ? -(a) : (a))

#define NUM_RANDVALS 2000
#define PDS_DEPOT                  0
#define PDS_PI                     3.14159265358979323846264
#define PDS_INFINITY                1.8E+208 //(1<<30)//=1073741824
#define PDS_INFINITY_INT            (1<<30)//=1073741824
#define PDS_INFEASIBLE              PDS_INFINITY   
#define PDS_MAX_NUM_SUPPLYPOINTS    100 
#define PDS_MAX_NUM_TRIPSVEH        PDS_MAX_NUM_SUPPLYPOINTS *2 //max #trips per vehicles(=segments)            
#define PDS_EPSILON                .00001
#define PDS_DEFAULT_DEVIATION      .01
#define PDS_MAX_NUM_TRIPS        10000
#define F                          500



#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cmath>
//#include <time.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>


#include "PDSUtils.h"
#include "VRPNode.h"
#include "NodeC2C.h"
#include "VRPNodeSP.h"
#include "VRPNodeWS.h"
#include "VRPTrip.h"
#include "VRPSegment.h"
#include "Problem.h"
#include "Solution.h"
#include "quick_sort.h"
#include "quick_sort_DECREASE.h"
#include "quick_sort_arr.h"
#endif



