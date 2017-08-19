/*
 *  ATMS 502 / CSE 566 -- Spring, 2017
 *  Demo for pgm1:  Linear and nonlinear advection
 *  =====>>>>> PUT YOUR NAME HERE! and in "name" variable below <<<<<=====
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncarg/ncargC.h>
#include <ncarg/gks.h>
#define IWTYPE 1
#define WKID   1

main()
{

/*
 * Definitions
 */

#define NX 251
#define NY 251
#define BC_WIDTH 3
#define I1X BC_WIDTH
#define I1Y BC_WIDTH
#define I2X I1X+NX-1
#define I2Y I1Y+NY-1
#define NXDIM NX+2*BC_WIDTH
#define NYDIM NY+2*BC_WIDTH
#define MAXSTEP 800

char *name  = "David Villarreal";
/*char *title = "Program 1"; used elsewhere below */

/* Arrays and other variables */

	float s1[NXDIM][NYDIM],s2[NXDIM][NYDIM],strue[NX][NY], u[NX+1][NY],v[NX][NY+1];
	float strace[MAXSTEP],dt,courant,c,dx,dy,max,min,EDISS,EDIS,ET;
	float pi = 4.0*atan(1.0);
	int i,j,n,nstep,nplot;
	char plottitle[20];

	
	dx = (float)1.0/(float)(NX-1);
	dy = (float)1.0/(float)(NY-1);
	/*dt = (float)pi/(float)600.; */
	/*dt = (float)0.005;
	nstep = 700;*/
	
	dt = (float)0.002;
	/*nstep = 40;*/
	
	float * smax[nstep];
	float * smin[nstep];
	char advection_type, reply[5];



/* Variables to reverse default black/white colors in NCAR Graphics */

	Gcolr_rep rgb1,rgb2;

/* Function prototype declarations */

	void ic(int n1, int n2,int nx, int ny, int i1, int i2, int j1, int j2, float s[n1][n2], float u[nx+1][ny],float v[nx][ny+1],
	float dx,float dy);
	void bc(int n1, int n2, int i1, int i2, int j1, int j2, float s[n1][n2]);
	void advect1d(int N,int n, int i1, int i2, float s1[N],float s2[N],float u[n+1],float dt,float dx, char advection_type);
	void error(int nx, int ny, float st[][ny], float s[][ny],float ediss,float edis, float et);
	
	void advection(int nxdim, int nydim,int nx, int ny, int i1,int i2,int j1,int j2,
	float s1[nxdim][nydim],float s2[nxdim][nydim],
	float u[nx+1][ny], float v[nx][ny+1],
	float dx,float dy,float dt,
	char advection_type);
	
	void contr(int nx,int ny,float splot[nx][ny],float cint,float simtime,
           char *title,int colors,int pltzero,int nestX1,int nestX2,
           int nestY1,int nestY2,char *name);
	void sfc(int nx,int ny,int nymax,float splot[nx][ny],float simtime,
         float angh,float angv,char *label,char *name);
         
	void stats(int nx,int ny, int i1, int i2, int j1, int j2,float s[][ny],float * max,float* min);
	
/* Plotting declarations */
	int colors,pltzero;
	char title[25];
	float cint,simtime,angh,angv,contint;
	float splot[NX][NY];


/* Parameters and input .................................... */

	printf("Program #2       Numerical Fluid Dynamics\n\n");
	/*printf("NX=%d, BC_WIDTH=%d, I1=%d, I2=%d, NXDIM=%d\n",
		NX,BC_WIDTH,I1X,I2X,NXDIM);*/

/* Read in from user: */
	printf("Enter 1 for 2nd ord crowley; 2 for 6th ord crowley; or 3 for Takacs: ");
	scanf("%s",reply);
	advection_type = reply[0];
	printf("Enter total number of desired steps");
	scanf("%d",&nstep);
	printf("Enter how often you want to plot");
	scanf("%d",&nplot);
	/*printf("enter desired contour interval: ");
	scanf("%f",&contint);*/
	
/*
 * Open the NCAR Graphics package and set colors.
 */
	gopen_gks("stdout",0);
	gopen_ws(WKID, NULL, IWTYPE);
	gactivate_ws(WKID);

	/* omit following four lines to invert black/white colors */
	rgb1.rgb.red = 1.; rgb1.rgb.green = 1.; rgb1.rgb.blue = 1.;
	rgb2.rgb.red = 0.; rgb2.rgb.green = 0.; rgb2.rgb.blue = 0.;
        gset_colr_rep(WKID,0,&rgb1);
        gset_colr_rep(WKID,1,&rgb2);

	ic(NXDIM,NYDIM,NX,NY,I1X,I2X,I1Y,I2Y,s1,u,v,dx,dy);
	

	printf("Plotting initial S.\n");	
	for (j=I1Y; j<=I2Y; j++) {	/* copy s1 to NX,NY plotting array */
	  for (i=I1X; i<=I2X; i++) {
	    splot[i-I1X][j-I1Y] = s1[i][j]; 
	    strue[i-I1X][j-I1Y] = s1[i][j]; 
	    } 
	} /* works for any BC_WIDTH */
	cint    = 1.0;	/* contour interval */
	colors  =   0;	/* positive values colored red, negative blue, all solid */
	pltzero =  -1;	/* don't plot the often noisy zero line */
	simtime = 0.0;
	contr(NX,NY,splot,cint,simtime,"Initial S",colors,pltzero,0,0,0,0,name);
	sfc(NX,NY,NY,splot,simtime,30.,50.,"Initial S ",name);

/* Here we create adresses from * smin/smax*/	
	float bla1[nstep];
	float bla2[nstep];
	for(i=0;i<nstep;i++){
	smax[i] = &(bla1[i]);
	smin[i] = &(bla2[i]);
	}
	
	/*begin the integration of our initial field*/
	for(n=1;n<=nstep;n++){
	bc(NXDIM,NYDIM,I1X,I2X,I1Y,I2Y,s1);
	advection(NXDIM,NYDIM,NX,NY, I1X,I2X,I1Y,I2Y, s1, s2, u,  v, dx, dy, dt, advection_type);
	stats(NXDIM,NYDIM, I1X, I2X, I1Y, I2Y,s1, smax[n-1], smin[n-1]);
	
	if(n%nplot==0){
	
	for (j=I1Y; j<=I2Y; j++) {	/* copy s1 to NX,NY plotting array */
	  for (i=I1X; i<=I2X; i++) {
	    splot[i-I1X][j-I1Y] = s1[i][j];
	   }     
	}
		
	colors  =   0;	
	pltzero =  -1;	
	simtime = (float)n*dt;
	sprintf(plottitle,"N = %i",n);
	contr(NX,NY,splot,1.0,0.0,plottitle,0,-1,0,0,0,0,name);
	sfc(NX,NY,NY,splot,simtime,30.,50.,plottitle,name);
	
	}

	}
	
		
	cint    = 0.5;	
	colors  =   0;	
	pltzero =  -1;	
	simtime = (float)nstep*dt;
	contr(NX,NY,splot,1.0,0.0,"Final S",0,-1,0,0,0,0,name);
	/*contr(NX,NY,splot,contint,simtime,"FINAL S ",colors,pltzero,0,0,0,0,name);*/
	sfc(NX,NY,NY,splot,simtime,30.,50.,"Final S ",name);
	

	cint    = 0.1;	
	colors  =   0;	
	pltzero =   0;	
	simtime = 0.0;
	contr(NX+1,NY,u,cint,simtime,"Initial U",colors,pltzero,0,0,0,0,name);


	cint    = 0.1;	/* contour interval */
	colors  =   0;	/* positive values colored red, negative blue, all solid */
	pltzero =   0;	/* do plot the zero line */
	simtime = 0.0;
	contr(NX,NY+1,v,cint,simtime,"Initial V",colors,pltzero,0,0,0,0,name);
	
	
	
	

	c_ezy(smax[0], nstep, "max" );
	c_ezy(smin[0], nstep, "min" );
	


 

	gdeactivate_ws(WKID);
	gclose_ws(WKID);
	gclose_gks();

	printf("Program finished normally.\n");
	printf("%.4f time step\n",dt);
	exit;
}
