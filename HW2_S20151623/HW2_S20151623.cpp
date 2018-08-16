#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"my_quick_sorts.h" 

int cmp(const void *a, const void *b) { return ((ELEMENT *)a)->score - ((ELEMENT *)b)->score; }

int main()
{
	FILE *fp, *fin, *fout;
	int op, n, i;
	char in[99], out[99];				// for storing .bin files name
	ELEMENT *A;
	void (*QsortPtr[5]) (void *, size_t, size_t, _Cmpfun *) = {
		qsort,							// From "stdlib.h"
		qsort_orig,
		qsort_median_insert,
		qsort_median_insert_iter,
		qsort_final
	};
	char algo_name[5][30] = {
		"qsort",
		"qsort_orig",
		"qsort_median_insert",
		"qsort_median_insert_iter",
		"qsort_final"
	};
	clock_t before;
	double result[11], sum=0;
	// Declaring variables
	fp = fopen("HW2_commands.txt", "r");
	fscanf(fp, "%d%d%s%s", &op, &n, in, out);
	fclose(fp);
	// Get info from file
	op -= op > 20 ? 20 : 1; 			// makes 21, 22, 23, 24 as 1, 2, 3, 4 respectively
	A = (ELEMENT *)malloc(sizeof(ELEMENT) * n);
	// Initialize A ( array ) and op ( given function number )
	for(i = 0; i < 10; i++)
	{
		fin = fopen(in, "rb");
		fread(A, sizeof(ELEMENT), n, fin);
		fclose(fin);
		// Get array's elements
		before = clock();
		QsortPtr[op](A, n, sizeof(ELEMENT), cmp);
		result[i] = (double)(clock() - before)/CLOCKS_PER_SEC;
		// Sort!
		sum += result[i];
	}
	fout = fopen(out, "wb");
	fwrite(A, sizeof(ELEMENT), n, fout);
	fclose(fout);
	// Write elements of sorted array
	free(A);
	// Free which was dynamically allocated
	printf("Input size : %10d\nWith algorithm %d, '%s'\n", n, op?op+20:1, algo_name[op]);
	puts("Sorted 10 times");
	for(i = 0; i < 10; i++) printf("%d : %4.3fms\n", i+1, result[i] * 1000);
	printf("Average : %4.3fms\n", sum * 100);
	return 0;
}
