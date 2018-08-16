#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#define min(x, y) (((x)>(y))?(y):(x))

struct I{
	int i, j;
}idx[222];

int cmp(struct I *a, struct I *b) { return a->i^b->i?a->i-b->i:a->j-b->j; }

int n, pair[222][222], top;
double x[222], y[222], d[222][222];

double dist(int a, int b)
{
	if(abs(a-b) == 1 || abs(a-b) == n-1) return 0;
	return sqrt((x[a]-x[b])*(x[a]-x[b])+(y[a]-y[b])*(y[a]-y[b]));
}

double tri(int f, int s)
{
	if(s < 4) return 0.0;
	if(d[f][s]) return d[f][s];
	// cutting & memoization

	int i;
	double ret = 1.7e307, a, b, c;
	for(i = 1; i < s-1; i++)
	{
		a = tri(f, i+1);
		b = tri(f+i, s-i);
		c = dist(f, f+i) + dist(f+i, f+s-1);
		if(ret > a+b+c)
		{
			ret = a+b+c;
			pair[f][s] = i;
		}
	}
	d[f][s] = ret;
	return ret;
}

void find_pairs(int f, int s)
{
	if(s < 4) return;
	int cur = pair[f][s];
	if(cur > 1)
	{
		idx[top].i = f;
		idx[top++].j = f+cur;
	}
	if(s-cur > 2)
	{
		idx[top].i = f+cur;
		idx[top++].j = f+s-1;
	}
	if(s < n)
	{
		idx[top].i = f;
		idx[top++].j = f+s-1;
	}
	find_pairs(f, cur+1);
	find_pairs(f+cur, s-cur);
}

int main()
{
	FILE *fp, *tcfp;
	int t, tc, n, i, j;
	double a;
	char in[22], out[22];

	fp = fopen("PT_test_command.txt", "r");
	fscanf(fp, "%d\n", &tc);
	for(t = 0; t < tc; t++)
	{
		fscanf(fp, "%s%s", in, out);
		// read input file name & output file name 

		tcfp = fopen(in, "r");
		fscanf(tcfp, "%d", &n);
		for(i = 0; i < n; i++)
			fscanf(tcfp, "%lf%lf", x+i, y+i);
		fclose(tcfp);
		// get points

		top = 0;
		memset(d, 0.0, sizeof(d));
		memset(pair, 0, sizeof(pair));
		a = tri(0, n);
		find_pairs(0, n);
		// set dp table

		qsort(idx, top, sizeof(struct I), cmp);
		// sort indices

		tcfp = fopen(out, "w");
		fprintf(tcfp, "%lf\n", a);
		for(i = 0; i < top; i++)
			fprintf(tcfp, "%d %d\n", idx[i].i, idx[i].j);
		fclose(tcfp);
		// write the answer with given points
	}
	fclose(fp);
	return 0;
}
