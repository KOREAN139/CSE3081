#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

bool d[222][111111];

int main()
{
	FILE *fp, *tcfp;
	int t, tc, n, L, i, j, f, set[222], sel[222], idx;
	char in[22], out[22];
	fp = fopen("SS_test_command.txt", "r");
	fscanf(fp, "%d", &tc);
	for(t = 0; t < tc; t++)
	{
		fscanf(fp, "%s%s", in, out);
		// read input file name & output file name
		
		idx = 0;
		tcfp = fopen(in, "r");
		fscanf(tcfp, "%d", &n);
		for(i = 0; i < n; i++)
			fscanf(tcfp, "%d", set+i);
		fscanf(tcfp, "%d", &L);
		fclose(tcfp);
		// read n, set, L

		for(i = 0; i < n; i++)
			for(j = 0; j <= L; j++)
				d[i][j] = 0;
		//initailize dp table

		for(i = 0; i < n; i++)
		{
			d[i][0] = d[i][set[i]] = 1;
			for(j = 1; j <= L; j++)
			{
				if(i) d[i][j] = d[i-1][j];
				if(i && j >= set[i]) d[i][j] |= d[i-1][j-set[i]];
			}
		}
		// set and fill n*L size dp table

		tcfp = fopen(out, "w");
		if(L)
		{
			fprintf(tcfp, "%d\n", d[n-1][L]);
			if(d[n-1][L])
			{
				for(i = n; i--; )
				{
					if(i && d[i-1][L]) continue;
					if(!i && !L) continue;
					L -= set[i];
					sel[idx++] = i;
				}
				fprintf(tcfp, "%d\n", idx);
				while( idx-- )
					fprintf(tcfp, "%d\n", sel[idx]);
			}
		} else
			fprintf(tcfp, "0\n");
		fclose(tcfp);
		// write the answer with given set & L
	}
	fclose(fp);
	return 0;
}
