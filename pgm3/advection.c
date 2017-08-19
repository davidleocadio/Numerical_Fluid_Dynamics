

void advection(int NXDIM, int NYDIM,int nx, int ny, int i1,int i2,int j1,int j2,
	float s1[NXDIM][NYDIM],float s2[NXDIM][NYDIM],
	float u[nx+1][ny], float v[nx][ny+1],
	float dx,float dy,float dt,
	char advection_type)

{
	
	int i,ii,j;
	float s11d[NXDIM],s21d[NXDIM],u1d[nx+1];
	void bc(int n1, int n2, int i1, int i2, int j1, int j2, float s[n1][n2]);
	void advect1d(int N,int n, int i1, int i2, float s1[N],float s2[N],float u[n+1],float dt,float dx, char advection_type);

/*printf("BFJ: advection-x\n");*/

	/*x integration*/
	for(j = j1; j<=j2;j++){
		for(ii=0;ii<NXDIM;ii++){s11d[ii]=s1[ii][j];}
		for(ii=0;ii<nx+1;ii++){u1d[ii]=u[ii][j-j1];}
		advect1d(NXDIM,nx, i1, i2,s11d,s21d,u1d,dt,dx,advection_type);
		for(i=i1;i<=i2;i++){
		s2[i][j]=s21d[i];
		}
	}
	
/*printf("BFJ: advection-y\n");*/
	/*y integration*/
	/* BFJ bc( NXDIM,  NYDIM,  s2); */
	bc(NXDIM,NYDIM,i1,i2,j1,j2,s2);
	
	for(j = i1; j<=i2;j++){
		for(ii=0;ii<NYDIM;ii++){s11d[ii]=s2[j][ii];}
		for(ii=0;ii<ny+1;ii++){u1d[ii]=v[j-i1][ii];}
		advect1d(NYDIM,ny,j1,j2,s11d,s21d,u1d,dt,dy,advection_type);
		for(i=j1;i<=j2;i++){
		s2[j][i]=s21d[i];
		}
	}
	
	
	
	
	for (i=0; i<=NXDIM-1; i++){
		for(j=0;j<=NYDIM-1;j++){
			 
	  		s1[i][j] = s2[i][j];

		}
	}
	

	return;
}

