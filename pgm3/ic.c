/*
 * ============================ ic =====================
 * IC sets the initial condition
 * Arguments:
 *
 *	s1	real array	IC data. Set i1..i2 here;
 *				  [i1-1],[i2+1] = ghost zones
 *				  if 1 ghost point on each side
 *	dx	real		grid spacing
 *	i1,i2	integers	indices bounding array data
 *	nx	integer		number of physical grid points
 *
 */
#include <math.h>

void ic(int NXDIM, int NYDIM, int nx, int ny, int i1, int i2, int j1, int j2, float s1[NXDIM][NYDIM], 
	float u[nx+1][ny], float v[nx][ny+1], float dx,float dy)
{
	int i,j;
	float x,y,pi,d,r;

	pi = 4.0*atan(1.0);

 	/* In loop below:
 	 * i runs i1...i2 and is actual array offset/memory adddress
 	 * i=i1 is the first physical point; i-i1 used to find X value
 	 */

	for (i=i1; i<=i2; i++) {
	x = -0.5+(i-i1)*dx;
		for(j=j1;j<=j2;j++){
		y = -0.5+(j-j1)*dy;
		d = sqrt(pow(x,2)+pow(y+0.05,2));
		r = 0.125;
		if (d>r){
		s1[i][j] = 0.;
		}else{
		s1[i][j] = 5.*(1.+cos(pi*d/r));	 }

							}
}

	for(i=0;i<nx+1;i++){
	 x = -0.5-dx/2.0+i*dx;
		for(j=0;j<ny;j++){
	y = -0.5+j*dy;
	u[i][j] = (float)sin(4.*pi*x)*sin(4.*pi*y);

	}
	}
	
	for(i=0;i<nx;i++){
	x = -0.5+i*dx;
		for(j=0;j<ny+1;j++){
		y = -0.5-dy/2.0+j*dy;
	v[i][j] = (float)cos(4.*pi*x)*cos(4.*pi*y);

	}
	}
	
	
	
/*	for(i=0;i<nx+1;i++){
		for(j=0;j<ny;j++){
	y = -0.5+j*dy;
	u[i][j] = -2*y;
	}
	}
	
	for(i=0;i<nx;i++){
	x = -0.5+i*dx;
		for(j=0;j<ny+1;j++){
	v[i][j] = 2*x;
	}
	}*/
		
		
						
							

	return;
}

