
/*advect1d(s11d,s21d,u1d,advection_type);*/
void advect1d(int N,int n,int i1, int i2, float s1[N],float s2[N],float u[n+1],float dt,float dx, char advection_type){

int i,j;
float courant;


/* DO AVERAGING*/
for(i = 0 ; i < n;i++){
u[i] = 0.5*(u[i+1]+u[i]);
}





if(advection_type == '1'){
	for(i=i1;i<=i2;i++){
	courant = u[i-i1]*dt/dx;
	s2[i] = s1[i]-courant/2.*(s1[i+1]-s1[i-1])+courant*courant/2.*\
	    (s1[i+1]-2.*s1[i]+s1[i-1]);
	}
/*printf("did advect\n");*/
}


if(advection_type == '2'){
	for(i=i1;i<=i2;i++){
	courant = u[i-i1]*dt/dx;
	s2[i] = s1[i]+courant/60*(s1[i-3]-9*s1[i-2]+45*s1[i-1]-45*s1[i+1]+9*s1[i+2]-s1[i+3])\
	+pow(courant,2)/360*(2*s1[i-1]-27*s1[i-2]+270*s1[i-1]-490*s1[i]+270*s1[i+1]-\
	27*s1[i+2]+2*s1[i+3])+\
	pow(courant,3)/48*(-s1[i-3]+8*s1[i-2]-13*s1[i-1]+13*s1[i+1]-8*s1[i+2]+s1[i+3])+\
	pow(courant,4)/144*(-s1[i-3]+12*s1[i-2]-39*s1[i-1]+56*s1[i]-39*s1[i+1]+12*s1[i+2]-\
	s1[i+3])+\
	pow(courant,5)/240*(s1[i-3]-4*s1[i-2]+5*s1[i-1]-5*s1[i+1]+4*s1[i+2]-s1[i+3])+\
	pow(courant,6)/720*(s1[i-3]-6*s1[i-2]+15*s1[i-1]-20*s1[i]+15*s1[i+1]-6*s1[i+2]+s1[i+3]);
	}
/*printf("did advect\n");*/
}


if(advection_type == '3'){
	for(i=i1;i<=i2;i++){
	courant = u[i-i1]*dt/dx;
	if(courant>=0){
	s2[i] = s1[i]-courant/2.*(s1[i+1]-s1[i-1])+pow(courant,2)/2.*\
	    (s1[i+1]-2.*s1[i]+s1[i-1])-(1.+courant)/6.*courant*(courant-1)*\
	    (s1[i+1]-3.*s1[i]+3.*s1[i-1]-s1[i-2]);
	}
	
	if(courant < 0){
	s2[i] = s1[i]-courant/2.*(s1[i+1]-s1[i-1])+pow(courant,2)/2.*\
	    (s1[i+1]-2.*s1[i]+s1[i-1])-(1.+fabs(courant))/6.*courant*(courant+1.)*\
	    (s1[i-1]-3.*s1[i]+3.*s1[i+1]-s1[i+2]);
	}
	
	   
	}
/*printf("did advect\n");*/
}



return;

}