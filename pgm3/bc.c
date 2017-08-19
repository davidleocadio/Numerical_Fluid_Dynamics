/*
 * ============================ bc =====================
 * BC sets the boundary conditions
 * Arguments:
 *
 *	s1	real array	values at current time step
 *	i1,i2	integers	indices bounding array data
 *	nx	integer		main array size, not including
 *				extra 'ghost' zones/points
 */

/* BFJ add args */
void bc(int NXDIM, int NYDIM, int i1,int i2,int j1,int j2, float s[NXDIM][NYDIM])
{	
	int i,j;
	/* No ... don't hardcode, or do as little as possible
	int i1 = 1;
	int i2 = NXDIM-2;
	*/
	
/*printf("bc called with %d %d, %d %d %d %d\n",NXDIM,NYDIM,i1,i2,j1,j2);*/
	/* top, bottom */
/*printf("top,bottom\n");*/
	for( i = i1; i <= i2; i++){
		s[i][j1-1] = s[i][j1];
		s[i][j1-2] = s[i][j1];
		s[i][j1-3] = s[i][j1];
		s[i][j2+1] = s[i][j2];
		s[i][j2+2] = s[i][j2];
		s[i][j2+3] = s[i][j2];
	}

	/* left,right */
/*printf("left,right\n");*/
	for( j = j1; j <= j2; j++){
/*printf("   i,j = %3d %3d\n",i,j);*/
		s[i1-1][j] = s[i1][j];
		s[i1-2][j] = s[i1][j];
		s[i1-3][j] = s[i1][j];
		s[i2+1][j] = s[i2][j];
		s[i2+2][j] = s[i2][j];
		s[i2+3][j] = s[i2][j];
	}

	return;
}


		/* BFJ
		s[0][i] = s[1][i];
		s[1][i] = s[0][i];
		s[i2+1][i] = s[i2][i];
		s[i][i2+1] = s[i][i2];
		*/
