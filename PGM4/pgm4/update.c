

void update(int NXDIM, int NYDIM,float s1[NXDIM][NYDIM], float s2[NXDIM][NYDIM]){
	
	int i,j;
	
	for (i=0; i<NXDIM; i++){
		for(j=0;j<NYDIM;j++){
			 
	  		s1[i][j] = s2[i][j];

		}
	}
	return;
}