
/*advect1d(s11d,s21d,u1d,advection_type);*/
void advect1d(int N,int n,int i1, int i2, float s1[N],float s2[N],float u[n+1],float dt,float dx, char advection_type){

int i,j;
float courant;


/* DO AVERAGING*/
for(i = 0 ; i < n;i++){
u[i] = 0.5*(u[i+1]+u[i]);
}




/*Coarse advection*/
if(advection_type == '1'){
	for(i=i1;i<=i2;i++){
	courant = u[i-i1]*dt/dx;
	s2[i] = s1[i]-courant/2.*(s1[i+1]-s1[i-1])+courant*courant/2.*\
	    (s1[i+1]-2.*s1[i]+s1[i-1]);
	}

}

/*Fine advection*/

if(advection_type == '0'){
	for(i=i1+1;i<=i2-1;i++){
	courant = u[i-i1]*dt/dx;
	s2[i] = s1[i]-courant/2.*(s1[i+1]-s1[i-1])+courant*courant/2.*\
	    (s1[i+1]-2.*s1[i]+s1[i-1]);
	}

}






return;

}