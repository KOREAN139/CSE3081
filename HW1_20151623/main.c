#include<stdio.h>
#include<stdlib.h>

void Algorithm1(char *in, char *out)    // find max-sum subarray with O(n^2)
{
	FILE *fp;
	int n, *A, s, i, j, tmp, a[3];

	fp = fopen(in, "rb");   // open input file
	fread(&n, sizeof(int), 1, fp);  // get n
	A = (int *)malloc(sizeof(int)*n);       // dynamically allocate array A as size of n*sizeof(int)
	fread(A, sizeof(int), n, fp);
	fclose(fp);     // close input file

	s = 1 << 31;      // set s as least value in integer
	for (i = 0; i < n; i++)        // find max-sum subarray
	{
		tmp = 0;
		for (j = i; j < n; j++)
		{
			tmp += A[j];
			if (s < tmp)     // if current sum of subarray is bigger than s, update the info of answer
			{
				a[0] = s = tmp;
				a[1] = i;
				a[2] = j;
			}
		}
	}

	fp = fopen(out, "wb");  // open output file
	fwrite(a, sizeof(int), 3, fp);
	fclose(fp);     // close output file

	free(A);
}

int Algorithm2_recur(int *arr, int len, int f, int *s, int *e)
{
	if (len == 1) return arr[0];
	if (!len) return 0;
	int mid, left, right, lmax, rmax, tmp, i, center, to, from;
	mid = len / 2;
	left = Algorithm2_recur(arr, mid, f, s, e);
	right = Algorithm2_recur(arr + mid, len - mid, f + mid, s, e);
	lmax = rmax = -10000000;
	tmp = 0;
	for (i = mid; i >= 0; i--)
	{
		tmp += arr[i];
		if (lmax < tmp)
			lmax = tmp, from = i;
	}
	tmp = 0;
	for (i = mid + 1; i < len; i++)
	{
		tmp += arr[i];
		if (rmax < tmp)
			rmax = tmp, to = i;
	}
	center = lmax + rmax;
	if (left >= right && left >= center)
		return left;
	if (right > left && right > center)
		return right;
	*s = from + f; *e = to + f;
	return center;
}

void Algorithm2(char *in, char *out)    // find max-sum subarray with O(nlgn)
{
	FILE *fp;
	int *A, n, a[3];

	fp = fopen(in, "rb");   // open input file
	fread(&n, sizeof(int), 1, fp);  // get n
	A = (int *)malloc(sizeof(int)*n);
	fread(A, sizeof(int), n, fp);   // get info of array elements
	fclose(fp);     // close input file

	a[0] = Algorithm2_recur(A, n, 0, &a[1], &a[2]);

	fp = fopen(out, "wb");  // open output file
	fwrite(a, sizeof(int), 3, fp);  // write answers
	fclose(fp);     // close output file

	free(A);
}

void Algorithm3(char *in, char *out)    // find max-sum subarray with O(n)
{
	FILE *fp;
	int n, A, s, i, j, tmp, a[3];

	fp = fopen(in, "rb");   // open input file
	fread(&n, sizeof(int), 1, fp);  // get n

	s = 1 << 31;
	tmp = 0; i = 0;
	for (j = 0; j < n; j++)
	{
		fread(&A, sizeof(int), 1, fp);  // get info of array elements
		if (tmp > 0)
			tmp += A;
		else
		{
			tmp = A;
			i = j;
		}
		if (tmp > s)
		{
			a[0] = s = tmp;
			a[1] = i;
			a[2] = j;
		}
	}
	fclose(fp);     // close input file

	fp = fopen(out, "wb");  // open output file
	fwrite(a, sizeof(int), 3, fp);  // write the answer
	fclose(fp);     // close output file
}

void Algorithm4(char *in, char *out)    // find max-sum subrectangle with O(n^4)
{
	FILE *fp;
	int n, **A, **PS, s, i, j, k, l, tmp, local, a[5];

	fp = fopen(in, "rb");   // open input file
	fread(&n, sizeof(int), 1, fp);  // get n
	A = (int **)malloc(sizeof(int *)*n);
	PS = (int **)malloc(sizeof(int *)*(n + 1));
	for (i = 0; i < n; i++)
	{
		*(A + i) = (int *)malloc(sizeof(int)*n);
		*(PS + i) = (int *)calloc(n + 1, sizeof(int));
		fread(*(A + i), sizeof(int), n, fp);    // get info of array elements
	}
	*(PS + n) = (int *)calloc(n + 1, sizeof(int));
	fclose(fp);     // close input file

	for (i = 1; i <= n; i++)
		for (j = 1; j <= n; j++)
			PS[i][j] = A[i - 1][j - 1] - PS[i - 1][j - 1] + PS[i][j - 1] + PS[i - 1][j];

	s = 1 << 31;      // set s as least value in integer
	for (i = 0; i < n; i++)  // i as top-left
	{
		for (j = i; j < n; j++)  // j as bottom-right
		{
			for (k = 0; k < n; k++)  // k as top
			{
				for (l = k; l < n; l++)  // l as bottom
				{
					tmp = PS[l + 1][j + 1] - PS[l + 1][i] - PS[k][j + 1] + PS[k][i];
					if (tmp > s)
					{
						a[0] = s = tmp;
						a[1] = k; a[2] = i;
						a[3] = l; a[4] = j;
					}
				}
			}
		}
	}

	fp = fopen(out, "wb");  // open output file
	fwrite(a, sizeof(int), 5, fp);  // write the answer
	fclose(fp);     // close output file

	for (i = 0; i < n; i++)
		free(A[i]);
	free(A);
}

int Algorithm5_sub(int *tmp, int *s, int *e, int r)     // modified from Algorithm3 func
{
	int local = 0, max = 1 << 31, i, start;
	*e = 0xffffffff;
	for (i = start = 0; i < r; i++)
	{
		local += tmp[i];
		if (local < 0)
		{
			local = 0;
			start = i + 1;
		}
		else if (local > max)
		{
			max = local;
			*s = start;
			*e = i;
		}
	}
	if (*e != 0xffffffff)
		return max;

	// when all numbers are negative (in tmp array)
	max = tmp[0];
	*s = *e = 0;
	for (i = 1; i < r; i++)
		if (tmp[i] > max)
			max = tmp[i], *s = *e = i;
	return max;
}

void Algorithm5(char *in, char *out)    // find max-sum subrectangle with O(n^3)
{
	FILE *fp;
	int n, **A, i, j, k, local, *tmp, a[5], s, e;

	fp = fopen(in, "rb");   // open input file
	fread(&n, sizeof(int), 1, fp);  // get n
	A = (int **)malloc(sizeof(int *)*n);
	tmp = (int *)malloc(sizeof(int)*n);
	for (i = 0; i < n; i++)
	{
		*(A + i) = (int *)malloc(sizeof(int)*n);
		fread(*(A + i), sizeof(int), n, fp); // get info of array elements
	}
	fclose(fp);     // close input file

	a[0] = 1 << 31;   // set a[0] as least value in integer

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++) tmp[j] = 0;
		for (j = i; j < n; j++)
		{
			for (k = 0; k < n; k++)
				tmp[k] += A[k][j];      // partial sum array for every row between i-th and j-th col
			local = Algorithm5_sub(tmp, &s, &e, n);
			if (local > a[0])
			{
				a[0] = local;
				a[1] = s; a[2] = i;
				a[3] = e; a[4] = j;
			}
		}
	}

	fp = fopen(out, "wb");  // open output file
	fwrite(a, sizeof(int), 5, fp);  // write the answer
	fclose(fp);     // close output file

	for (i = 0; i < n; i++)
		free(A[i]);
	free(A);
}

int main()
{
	FILE *fp = fopen("HW1_config.txt", "r");
	char input[44], output[44];
	int tc, op;
	double execution_time;
	fscanf(fp, "%d", &tc);  // get the number of test cases
	while (tc--)   // for the given test cases
	{
		fscanf(fp, "%d %s %s", &op, input, output);     // get info
		
		switch (op) {
		case 1: Algorithm1(input, output); break;
		case 2: Algorithm2(input, output); break;
		case 3: Algorithm3(input, output); break;
		case 4: Algorithm4(input, output); break;
		case 5: Algorithm5(input, output); break;
		default: break;
		}
	}
	fclose(fp);
	return 0;
}