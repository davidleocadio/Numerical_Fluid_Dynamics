

void passive_test(int nxdim, int nydim, int i1, int i2, int j1, int j2, float s1[][nydim], int *i_center, int *j_center){

float max = 0;
int i,j;

for(i=i1;i<=i2;i++){
	for(j=j1;j<=j2;j++){
	
	if(s1[i][j]>=max){
	max = s1[i][j];
	*i_center = i;
	*j_center = j;

	}

}

}

return;



}