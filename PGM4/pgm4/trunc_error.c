#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void trunc_error(int nxdim, int nydim, int nx, int ny, int i1, int i2,int j1, int j2, float s1[nxdim][nydim], float uavg[nx][ny], 
float vavg[nx][ny],float dt,float dx,float dy, int * i_center, int * j_center){

int i,j,I1,I2,J1,J2;
float errx;
float erry;
float err[nxdim][nydim];
float cx; 
float cy;
float max = 0;


for(i=0;i<nxdim;i++){
	for(j=0;j<nydim;j++){
	err[i][j]=0.0;
	}
}




for(i=i1+2;i<=i2-2;i++){
for(j=j1+2;j<=j2-2;j++){
	cx = uavg[i-i1][j-j1];
	cy = vavg[i-i1][j-j1];
	errx = (s1[i+2][j]-s1[i+1][j]+2*s1[i-1][j]-s1[i-2][j])/(2*pow(dx,2))*(pow(cx,2)-1)*uavg[i-i1][j-j1]*pow(dx,2)/6;
	erry = (s1[i][j+2]-s1[i][j+1]+2*s1[i][j-1]-s1[i][j-2])/(2*pow(dy,2))*(pow(cy,2)-1)*vavg[i-i1][j-j1]*pow(dy,2)/6;
	
	if(fabs(errx)>fabs(erry)){
	
	err[i][j] = errx;}
		else{
	
	err[i][j] = erry;
	
	};
}
}

/*establish max error*/
for(i=i1+2;i<=i2-2;i++){
for(j=j1+2;j<=j2-2;j++){

if(err[i][j]>=max){
max = err[i][j];
}


}

}

/*i1 j1*/
for(i=i1+2;i<=i2-2;i++){
for(j=j1+2;j<=j2-2;j++){

if(err[i][j]/max >= 0.5){
I1 = i;
J1 = j;
j = 5*j2;
i = 5*i2;
}


}
}


/*FIND J2*/
for(j = J1; j <= j2-2;j++){

if(err[I1][j]/max >= 0.5){
J2 = j;
}

}


for(i = I1; i<=i2-2;i++){
if(err[i][J1]/max >= 0.5){
I2 = i;}
}

*i_center = 0.5*(I1+I2);
*j_center = 0.5*(J1+J2);
/*printf("%i, %i \n",*i_center,*j_center);*/







return;
}