/*
 *
 * ============================= nestwind ===========================
 *
 * ATMS 502/CSE 566   Brian Jewett, Univ. IL   Spring 2017
 *
 * Set the nested grid wind fields.  Note this routine does NOT
 * take your coarse grid u,v fields as input; it derives the
 * nest u,v fields directly from the nest location and the
 * coarse grid spacing.
 *
 * Arguments:
 *	nx,ny	 integers	Used for u,v dimensions
 *	unest	 real array	U field on nest, dimension (nx+1,ny)
 *	vnest	 real array	V field on nest, dimension (nx,ny+1)
 *	nestX1,	 integers	Bottom left corner of nest,
 *	 nestY1			  in coarse grid coordinates
 *	dx	 real		COARSE grid spacing
 *	ratio	 integer	Nest refinement factor
 *	icase	 integer	Flow: 1=rotation, 2=deformation;
 *				  used to create u,v fields.
 *				  Note deformational flow "k" parameter.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void nestwind(int nx, int ny, float unest[][ny], float vnest[][ny+1],
	      int nestX1,int nestY1,float dx,int ratio,int icase)
{
	int i,j;
	float x0,y0,dxnest,x,y,k,pi;
/*
 * ... coordinates of bottom left corner of nest
 */
	dxnest = dx/(float)ratio;
	x0 = -0.5 + dx*(float)(nestX1-1);
	y0 = -0.5 + dx*(float)(nestY1-1);
/*
 * ... case 1: rotational flow
 */
	if (icase == 1) {

	  for (j=0; j<ny; j++) {
	    y = y0 + dxnest*(float)(j);
	    for (i=0; i<=nx; i++) {
	      x = x0 + dxnest*(float)(i) - 0.5*dxnest;
	      unest[i][j] = -2.*y;
	    }
	  }

	  for (j=0; j<=ny; j++) {
	    y = y0 + dxnest*(float)(j) - 0.5*dxnest;
	    for (i=0; i<nx; i++) {
	      x = x0 + dxnest*(float)(i);
	      vnest[i][j] = 2.*x;
	    }
	  }
/*
 * ... case 2: deformational flow
 */
	} else if (icase == 2) {

	  pi = 4.*atan(1.0);
	  k  = 4.0*pi;		/* must match initial condition */

	  for (j=0; j<ny; j++) {
	    y = y0 + dxnest*(float)(j);
	    for (i=0; i<=nx; i++) {
	      x = x0 + dxnest*(float)(i) - 0.5*dxnest;
	      unest[i][j] = sin(k*(x+.5)) * sin(k*(y+.5));
	    }
	  }

	  for (j=0; j<=ny; j++) {
	    y = y0 + dxnest*(float)(j) - 0.5*dxnest;
	    for (i=0; i<nx; i++) {
	      x = x0 + dxnest*(float)(i);
	      vnest[i][j] = cos(k*(x+.5)) * cos(k*(y+.5));
	    }
	  }
/*
 * ... check for invalid icase value
 */
	} else {
	  printf("nestwind: invalid icase=%d\n",icase);
	  exit(1);
	}

	return;
}
