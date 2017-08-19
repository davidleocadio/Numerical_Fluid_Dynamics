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

#define NX 151
#define NY 151
#define BC_WIDTH 1
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

	float s1[NXDIM][NYDIM],s2[NXDIM][NYDIM],strue[NX][NY], u[NX+1][NY],v[NX][NY+1],
	unest[NX+1][NY],vnest[NX][NY+1], s1nest[NXDIM][NYDIM],smaxhist[NXDIM][NYDIM];
	float uavg[NX][NY], vavg[NX][NY];
	float strace[MAXSTEP],dt,courant,c,dx,dy,max,min;
	float EDISS=0.0;
	float EDIS=0.0;
	float ET=0.0;
	float pi = 4.0*atan(1.0);
	int i,j,n,nstep,nplot,moveinterval;
	char plottitle[20];
	int FB; 

	
	dx = (float)1.0/(float)(NX-1);
	dy = (float)1.0/(float)(NY-1);
	/*dt = (float)pi/(float)600.; */
	/*dt = (float)0.005;
	nstep = 700;*/
	
	/*dt = (float)pi/600;*/
	/*nstep = 40;*/
	
	/*float * smax[nstep];
	float * smin[nstep];*/
	char advection_type, reply[5];
	int nestX1; int nestX2; int nestY1; int nestY2; int ratio; int flag;
	int nestX1old; int nestX2old; int nestY1old; int nestY2old;
	float nestsize;



/* Variables to reverse default black/white colors in NCAR Graphics */

	Gcolr_rep rgb1,rgb2;

/* Function prototype declarations */

	void ic(int n1, int n2,int nx, int ny, int i1, int i2, int j1, int j2, float s[n1][n2], float u[nx+1][ny],float v[nx][ny+1],
	float dx,float dy);
	void bc(int n1, int n2, int i1, int i2, int j1, int j2, float s[n1][n2]);
	void advect1d(int N,int n, int i1, int i2, float s1[N],float s2[N],float u[n+1],float dt,float dx, char advection_type);
	void error(int nx, int ny, float st[][ny], float s[][ny],float ediss,float edis, float et);
	void update(int nxdim, int nydim,float s1[nxdim][nydim], float s2[nxdim][nydim]);
	void advection(int nxdim, int nydim,int nx, int ny, int i1,int i2,int j1,int j2,
	float s1[nxdim][nydim],float s2[nxdim][nydim],
	float u[nx+1][ny], float v[nx][ny+1],
	float dx,float dy,float dt,
	char advection_type);
	int i_center, j_center;
	
	void dointerp(int nx,int ny,int nxdim,int nydim, 
	 float coarse[][nydim],float nest[][nydim],
	 int i1       ,int i2       ,int j1       ,int j2,
         int nestX1   ,int nestX2   ,int nestY1   ,int nestY2,
	 int nstep    ,int ratio    ,int flag,
         int nestX1old,int nestX2old,int nestY1old,int nestY2old);
         
    void nestwind(int nx, int ny, float unest[][ny], float vnest[][ny+1],
	      int nestX1,int nestY1,float dx,int ratio,int icase);
         
         
	void trunc_error(int nxdim, int nydim, int nx, int ny, int i1, int i2,int j1, int j2, float s1[nxdim][nydim], float uavg[nx][ny], 
float vavg[nx][ny],float dt,float dx,float dy, int * i_center, int * j_center);
	
	void contr(int nx,int ny,float splot[nx][ny],float cint,float simtime,
           char *title,int colors,int pltzero,int nestX1,int nestX2,
           int nestY1,int nestY2,char *name);
	void sfc(int nx,int ny,int nymax,float splot[nx][ny],float simtime,
         float angh,float angv,char *label,char *name);
         
	/*void stats(int nx,int ny, int i1, int i2, int j1, int j2,float s[][ny],float * max,float* min);*/
	void passive_test(int nxdim, int nydim, int i1, int i2, int j1, int j2, float s1[][nydim], int *i_center, int* j_center);
	
	void trunc_error(int nxdim, int nydim, int nx, int ny, int i1, int i2,int j1, int j2, 
	float s1[nxdim][nydim], float uavg[nx][ny],float vavg[nx][ny],
float dt,float dx,float dy, int  *i_center, int *j_center);
	
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
	/*printf("Enter 1 for 2nd ord crowley; 2 for 6th ord crowley; or 3 for Takacs: ");
	scanf("%s",reply);*/
	advection_type = '1';
	printf("Enter total number of desired steps");
	scanf("%d",&nstep);
	printf("Enter how often you want to plot");
	scanf("%d",&nplot);
/*	printf("Enter refinement ratio");
	scanf("%d",&ratio);*/
	
	
	printf("Enter 1 for FB, or 0 for no-FB");
	scanf("%i",&FB);
	/*printf("enter desired contour interval: ");
	scanf("%f",&contint);*/
	dt = (float)pi/(float)nstep;
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
	
	
	for(i=0;i<NXDIM;i++){
	for(j=0;j<NYDIM;j++){
	smaxhist[i][j]=0.0;
	}
	}

	

	printf("Plotting initial S.\n");	
	for (j=I1Y; j<=I2Y; j++) {	/* copy s1 to NX,NY plotting array */
	  for (i=I1X; i<=I2X; i++) {
	    splot[i-I1X][j-I1Y] = s1[i][j]; 
	    strue[i-I1X][j-I1Y] = s1[i][j]; 
	    } 
	} /* works for any BC_WIDTH */
	cint    = 0.5;	/* contour interval */
	colors  =   0;	/* positive values colored red, negative blue, all solid */
	pltzero =  -1;	/* don't plot the often noisy zero line */
	simtime = 0.0;
	contr(NX,NY,splot,cint,simtime,"Initial S",colors,pltzero,0,0,0,0,name);
	sfc(NX,NY,NY,splot,simtime,15.,50.,"Initial S ",name);
	printf("init S plotted.\n");
	
	
	
	cint    = 0.1;	
	colors  =   0;	
	pltzero =   0;	
	simtime = 0.0;
	contr(NX+1,NY,u,cint,simtime,"Initial U ",colors,pltzero,0,0,0,0,name);


	cint    = 0.1;	/* contour interval */
	colors  =   0;	/* positive values colored red, negative blue, all solid */
	pltzero =   0;	/* do plot the zero line */
	simtime = 0.0;
	contr(NX,NY+1,v,cint,simtime,"Initial V",colors,pltzero,0,0,0,0,name);
	
		
/* Here we create adresses from * smin/smax*/	
/*	float bla1[nstep];
	float bla2[nstep];
	for(i=0;i<nstep;i++){
	smax[i] = &(bla1[i]);
	smin[i] = &(bla2[i]);
	}*/
	

if(FB==1){

	printf("Enter refinement ratio");
	scanf("%d",&ratio);

	printf("Enter nest movement interval");
	scanf("%i",&moveinterval);
/*create avg fields*/

for(i=0;i<NX;i++){
for(j=0;j<NY;j++){
uavg[i][j] = 0.5*(u[i][j]+u[i+1][j]);
vavg[i][j] = 0.5*(v[i][j]+v[i][j+1]);
}
}
	
	
	printf("calculated averages.\n");		
	
	/*begin the integration of our initial field*/
	
	
	
	
	bc(NXDIM,NYDIM,I1X,I2X,I1Y,I2Y,s1);

	i_center = 0;
	j_center = 0;
	trunc_error(NXDIM, NYDIM, NX, NY, I1X, I2X,I1Y, I2Y, s1, uavg, 
 vavg, dt, dx, dy,  &i_center,  &j_center);
 
 	nestsize = (NX-1)/ratio;                  
	nestX1  = i_center - nestsize/2;
	if(nestX1<I1X){nestX1=I1X;};
	nestX2  = nestX1 + nestsize;      
	if(nestX2>I2X){nestX2=I2X; nestX1 = nestX2-nestsize;};
	nestY1  = j_center - nestsize/2 ; 
	if(nestY1<I1Y){nestY1=I1Y;};
	nestY2  = nestY1 + nestsize;   
	if(nestY2>I2Y){nestY2=I2Y; nestY1=nestY2-nestsize;};
	
	contr(NX,NY,splot,0.5,0.0,"Initial S with NEST",0,-1,nestX1,nestX2,nestY1,nestY2,name);
	
	
	/*nest placement*/
	flag = 1;
	dointerp(NX,NY,NXDIM,NYDIM,s1,s1nest,I1X,I2X,I1Y,I2Y,
      	  nestX1,nestX2,nestY1,nestY2,n,ratio,flag,0,0,0,0);
      	  
    /*set boundary values*/  	  
    flag = 10;
	dointerp(NX,NY,NXDIM,NYDIM,s1,s1nest,I1X,I2X,I1Y,I2Y,
      	  nestX1,nestX2,nestY1,nestY2,n,ratio,flag,0,0,0,0);
      	  
      	  
    	for (j=I1Y; j<=I2Y; j++) {	/* plot init nested grid solution */
	  for (i=I1X; i<=I2X; i++) {
	    splot[i-I1X][j-I1Y] = s1nest[i][j]; 
	    } 
	}
      	  
    contr(NX,NY,splot,0.5,0.0,"Nested grid solution",0,-1,0,0,0,0,name);
      	  
	nestwind(NX,  NY,  unest,  vnest,
	       nestX1, nestY1, dx, ratio, 1);
	
	for(n=1;n<=nstep;n++){
	bc(NXDIM,NYDIM,I1X,I2X,I1Y,I2Y,s1);
	advection(NXDIM,NYDIM,NX,NY, I1X,I2X,I1Y,I2Y, s1, s2, u,  v, dx, dy, dt, '1');
	update(NXDIM,NYDIM,s1,s2);
	
	flag = 10;
	dointerp(NX,NY,NXDIM,NYDIM,s1,s1nest,I1X,I2X,I1Y,I2Y,
      	  nestX1,nestX2,nestY1,nestY2,n,ratio,flag,0,0,0,0);
	
	for(i = 1; i <= ratio; i++){
	flag = 10;
	dointerp(NX,NY,NXDIM,NYDIM,s1,s1nest,I1X,I2X,I1Y,I2Y,
      	  nestX1,nestX2,nestY1,nestY2,n,ratio,flag,0,0,0,0);
	advection(NXDIM,NYDIM,NX,NY, I1X,I2X,I1Y,I2Y, s1nest, s2, unest,  vnest, dx/ratio, dy/ratio, dt/ratio, '0');
	update(NXDIM,NYDIM,s1nest,s2);
	}
	
	/*interpolate back to coarse*/
	flag = 2;
	dointerp(NX,NY,NXDIM,NYDIM,s1,s1nest,I1X,I2X,I1Y,I2Y,
      	  nestX1,nestX2,nestY1,nestY2,n,ratio,flag,0,0,0,0);
      	  
      	  
        /*set boundary values NEW  	  
    flag = 10;
	dointerp(NX,NY,NXDIM,NYDIM,s1,s1nest,I1X,I2X,I1Y,I2Y,
      	  nestX1,nestX2,nestY1,nestY2,n,ratio,flag,0,0,0,0);  	  */
      	  
  /*  trunc_error(NXDIM, NYDIM, NX, NY, I1X, I2X,I1Y, I2Y, s1, uavg, 
 vavg, dt, dx, dy,  &i_center,  &j_center);  	*/
 
 
 

	if(n%moveinterval ==0){
	
	
	/*interpolate back to coarse*/
	/*flag = 2;
	dointerp(NX,NY,NXDIM,NYDIM,s1,s1nest,I1X,I2X,I1Y,I2Y,
      	  nestX1,nestX2,nestY1,nestY2,n,ratio,flag,0,0,0,0);*/
      	  
    trunc_error(NXDIM, NYDIM, NX, NY, I1X, I2X,I1Y, I2Y, s1, uavg, 
 vavg, dt, dx, dy,  &i_center,  &j_center);  	  
	
	
	
	nestX1old = nestX1;
	nestX2old = nestX2;
	nestY1old = nestY1;
	nestY2old = nestY2;
	
                 
	nestX1  = i_center - nestsize/2;
	if(nestX1<I1X){nestX1=I1X;};
	nestX2  = nestX1 + nestsize;      
	if(nestX2>I2X){nestX2=I2X; nestX1 = nestX2-nestsize;};
	nestY1  = j_center - nestsize/2 ; 
	if(nestY1<I1Y){nestY1=I1Y;};
	nestY2  = nestY1 + nestsize;   
	if(nestY2>I2Y){nestY2=I2Y; nestY1=nestY2-nestsize;};
	
	
	
	
	
	/*move the nest*/
	flag = -1;
	dointerp(NX,NY,NXDIM,NYDIM,s1,s1nest,I1X,I2X,I1Y,I2Y,
      	  nestX1   ,nestX2   ,nestY1   ,nestY2   ,n,ratio,flag,
      	  nestX1old,nestX2old,nestY1old,nestY2old);
    /*set boundaries*/  	  
    flag = 10;
	dointerp(NX,NY,NXDIM,NYDIM,s1,s1nest,I1X,I2X,I1Y,I2Y,
      	  nestX1,nestX2,nestY1,nestY2,n,ratio,flag,0,0,0,0);
    /*interpolate nested winds*/  	  
	nestwind(NX,  NY,  unest,  vnest,
	       nestX1, nestY1, dx, ratio, 1);  	  
      	  
      	  
	}
	
	
	if(n%nplot==0){
	
	for (j=I1Y; j<=I2Y; j++) {	
	  for (i=I1X; i<=I2X; i++) {
	    splot[i-I1X][j-I1Y] = s1[i][j];
	   }     
	}
	simtime = (float)n*dt;
	sprintf(plottitle,"N = %i",n);
	contr(NX,NY,splot,0.5,0.0,plottitle,0,-1,nestX1,nestX2,nestY1,nestY2,name);
	}
	
	/*passive_test(NXDIM, NYDIM, I1X, I2X, I1Y, I2Y, s1,  &i_center,  &j_center);
	smaxhist[i_center][j_center]=s1[i_center][j_center];*/
	
	for(i=I1X;i<=I2X;i++){
		for(j=I1Y;j<=I2Y;j++){
		if(s1[i][j]>smaxhist[i][j]){smaxhist[i][j]=s1[i][j];}	
		}
	}	

	
	
	
	}
	
	
	
	
	
	for (j=I1Y; j<=I2Y; j++) {	/* copy s1 to NX,NY plotting array */
	for (i=I1X; i<=I2X; i++) {
	    splot[i-I1X][j-I1Y] = s1[i][j];
	   }  
	   } 
	
	
	
	
	contr(NXDIM,NYDIM,smaxhist,cint,simtime,"max S with FB",colors,pltzero,0,0,0,0,name);
	sfc(NXDIM,NYDIM,NYDIM,smaxhist,simtime,15.,50.,"max S with FB",name);	
	
	
		
	cint    = 0.5;	
	colors  =   0;	
	pltzero =  -1;	
	simtime = (float)nstep*dt;
	contr(NX,NY,splot,0.5,0.0,"Final S w/ FB",0,-1,nestX1,nestX2,nestY1,nestY2,name);
	/*contr(NX,NY,splot,contint,simtime,"FINAL S ",colors,pltzero,0,0,0,0,name);*/
	sfc(NX,NY,NY,splot,simtime,15.,50.,"Final S w/ FB ",name);
	error( NX,  NY, strue, splot,EDISS,EDIS, ET);
	
	
	for (j=I1Y; j<=I2Y; j++) {	/* plot init nested grid solution */
	  for (i=I1X; i<=I2X; i++) {
	    splot[i-I1X][j-I1Y] = s1nest[i][j]; 
	    } 
	}
      	  
    contr(NX,NY,splot,0.5,0.0,"Final solution on nest",0,-1,0,0,0,0,name);

	

	
	}
	
	
	if(FB==0){
	
	
	for(n=1;n<=nstep;n++){
	
		bc(NXDIM,NYDIM,I1X,I2X,I1Y,I2Y,s1);
		advection(NXDIM,NYDIM,NX,NY, I1X,I2X,I1Y,I2Y, s1, s2, u,  v, dx, dy, dt, '1');
		update(NXDIM,NYDIM,s1,s2);
	
			if(n%nplot==0){
	
		for (j=I1Y; j<=I2Y; j++) {	
	  	for (i=I1X; i<=I2X; i++) {
	    splot[i-I1X][j-I1Y] = s1[i][j];
	   }     
		}
			

	simtime = (float)n*dt;
	sprintf(plottitle,"N = %i",n);
	contr(NX,NY,splot,0.5,0.0,plottitle,0,-1,0,0,0,0,name);
	}
	
	/*passive_test(NXDIM, NYDIM, I1X, I2X, I1Y, I2Y, s1,  &i_center,  &j_center);
	smaxhist[i_center][j_center]=s1[i_center][j_center];*/
	
	for(i=I1X;i<=I2X;i++){
		for(j=I1Y;j<=I2Y;j++){
		if(s1[i][j]>smaxhist[i][j]){smaxhist[i][j]=s1[i][j];}	
		}
	}	

	
	}
	
		for (j=I1Y; j<=I2Y; j++) {	/* copy s1 to NX,NY plotting array */
	for (i=I1X; i<=I2X; i++) {
	    splot[i-I1X][j-I1Y] = s1[i][j];
	   }  
	   } 
	
	
	
	
	contr(NXDIM,NYDIM,smaxhist,cint,simtime,"max S no FB",colors,pltzero,0,0,0,0,name);
	sfc(NXDIM,NYDIM,NYDIM,smaxhist,simtime,15.,50.,"max S no FB",name);	
	
	
		
	cint    = 0.5;	
	colors  =   0;	
	pltzero =  -1;	
	simtime = (float)nstep*dt;
	contr(NX,NY,splot,0.5,0.0,"Final S no FB",0,-1,0,0,0,0,name);
	/*contr(NX,NY,splot,contint,simtime,"FINAL S ",colors,pltzero,0,0,0,0,name);*/
	sfc(NX,NY,NY,splot,simtime,15.,50.,"Final S no FB ",name);
	error( NX,  NY, strue, splot,EDISS,EDIS, ET);
	
	
	
	}
	
	
	
	
	
	
	
	
	
	
	

/*	c_ezy(smax[0], nstep, "max" );
	c_ezy(smin[0], nstep, "min" );*/
	


 

	gdeactivate_ws(WKID);
	gclose_ws(WKID);
	gclose_gks();

	printf("Program finished normally.\n");
	printf("%.4f time step\n",dt);
	exit;
}
