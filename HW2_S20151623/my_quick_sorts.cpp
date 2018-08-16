#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"my_quick_sorts.h"

void swap(char *x, char *y, size_t size)
{
	char buf[MAX_BUF_SIZE];
	size_t m, ms;
	for(ms = size; ms > 0; ms-=m, x+=m, y+=m)
	{
		m = (ms < sizeof(buf)) ? ms : sizeof(buf);
		memcpy(buf, x, m);
		memcpy(x, y, m);
		memcpy(y, buf, m);
	}
}

char* med3(char *a, char *b, char *c, _Cmpfun *cmp)
{
	return cmp(a, b) < 0 ?
		(cmp(b, c) < 0 ? b : (cmp(a, c) < 0 ? c : a))
		:(cmp(b, c) > 0 ? b : (cmp(a, c) < 0 ? a : c));
}

size_t median3(size_t a, size_t b, size_t c)
{ return a>b?a>c?b>c?b:c:a:a<c?b<c?b:c:a; }

size_t median(size_t j, size_t k)
{
	size_t a, b, c;
	a = rand() % (k-j) + j;
	b = rand() % (k-j) + j;
	c = rand() % (k-j) + j;
	return median3(a, b, c);
}

size_t partition(void *A, size_t n, size_t size, _Cmpfun *cmp, size_t pivot)
{
	char *S = (char *)A;
	char *p = (char *)malloc(sizeof(char) * size);
	size_t i=0, k=0;
	memcpy(p, S+pivot*size, size);
	swap(S+pivot*size, S+(n-1)*size, size);
	for(i = 0; i < (n-1)*size; i+=size)
	{
		if((*cmp)(S+i, p) <= 0)
		{
			swap(S+k*size, S+i, size);
			k++;
		}
	}
	swap(S+(n-1)*size, S+k*size, size);
	free(p);
	return k;
}

void insertion_sort(void *A, size_t n, size_t size, _Cmpfun *cmp)
{
	size_t i, j;
	char *S = (char *)A;
	char *T = (char *)malloc(sizeof(char) * size);
	i = n-1;
	while(i-- > 0)
	{
		memcpy(T, S+i*size, size);
		j = i;
		while(++j < n && (*cmp)(T, S+j*size) > 0);
		if(--j == i) continue;
		memcpy(S+i*size, S+(i+1)*size, size*(j-i));
		memcpy(S+j*size, T, size);
	}
	free(T);
	return;
}

void qsort_orig(void *A, size_t n, size_t size, _Cmpfun *cmp)
{
	char *S = (char *)A;
	size_t i, pivot;
	if( n > 1 )
	{
		pivot = partition(S, n, size, cmp, 0);
		// partition part ends here

		qsort_orig(S, pivot, size, cmp);
		qsort_orig(S+(pivot+1)*size, n-pivot-1, size, cmp);
		// recursion part
	}
	return;
}

void qsort_median_insert(void *A, size_t n, size_t size, _Cmpfun *cmp)
{
	char *S;
	size_t pivot;
	size_t p, fir, sec, thi, len, l, r;
	S = (char *)A;
	if(n < 20)
	{
		insertion_sort(S, n, size, cmp);
		return;
	}
	// Do insertion sort when given array's size is small enough
	pivot = median(0, n);
	// set pivot as median of randomly chosen 3 indices

	pivot = partition(S, n, size, cmp, pivot);
	// partition part ends here

	if(pivot) qsort_median_insert(S, pivot, size, cmp);
	if(n > pivot+1) qsort_median_insert(S+(pivot+1)*size, n-pivot-1, size, cmp);
	// recursion part
	return;
}

void qsort_median_insert_iter(void *A, size_t n, size_t size, _Cmpfun *cmp)
{
	char *S = (char *)A;
	char *fir, *sec, *thi, *P, *L, *R;
	size_t pivot, j = n-1, p, len;
	while(j > 0)
	{
		if(j < 7)
		{
			insertion_sort(S, j+1, size, cmp);
			return;
		}
		// Do insertion sort when given array's size is small enough
		// pivot = median(0, j);
		if(j > 6)
		{
			L = (char *)A;
			R = (char *)A + j*size;
			len = j+1;
			p = len/8;
			fir = med3(L, L+p*size, L+2*p*size, cmp);
			sec = med3(L+(len/2-p)*size, L+len/2*size, L+(len/2+p)*size, cmp);
			thi = med3(R-2*p*size, R-p*size, R, cmp);
			P = med3(fir, sec, thi, cmp);
			pivot = (size_t)(P-(char *)A)/size;
		}
		// set pivot as median of randomly chosen 3 indices
		pivot = partition(S, j+1, size, cmp, pivot);
		// partition part ends here

		if(pivot > j-pivot)
		{
			if(pivot < j) qsort_median_insert_iter(S+(pivot+1)*size, j-pivot, size, cmp);
			j = pivot-1;
			// When pivot is at right side
		} else
		{
			if(pivot) qsort_median_insert_iter(S, pivot, size, cmp);
			S += (pivot+1)*size;
			j -= pivot+1;
			// When pivot is at middle or left side
		}
	}
	return;
}

void qsort_final(void *A, size_t n, size_t size, _Cmpfun *cmp)
{
	char *S = (char *)A;
	char *t = (char *)malloc(sizeof(char) * size);
	char *fir, *sec, *thi, *pivot, *L, *R;
	size_t top = 0, l, r, len, p;
	size_t I, J, P, Q, k;
	stack s[MAX_STACK_SIZE];
	s[++top].l = 0; s[top].r = n-1;
	while(top)
	{
		l = s[top].l; r = s[top--].r;
		if(l >= r) continue;
		len = r-l+1;
		if(len <= 8)
		{
			insertion_sort(S+l*size, len, size, cmp);
			continue;
		}
		L = (char *)A + l*size;
		R = (char *)A + r*size;
		p = len/8;
		fir = med3(L, L+p*size, L+2*p*size, cmp);
		sec = med3(L+(len/2-p)*size, L+len/2*size, L+(len/2+p)*size, cmp);
		thi = med3(R-2*p*size, R-p*size, R, cmp);
		pivot = med3(fir, sec, thi, cmp);
		swap(pivot, S+l*size, size);
		I = l; J = r+1; P = l; Q = r+1;
		while(1)
		{
			memcpy(t, S+l*size, size);
			while((*cmp)(S+(++I)*size, t) < 0)
				if(I == r) 
					break;
			while((*cmp)(t, S+(--J)*size) < 0)
				if(J == l)
					break;
			if(I >= J)
				break;
			swap(S+I*size, S+J*size, size);
			if((*cmp)(t, S+I*size) == 0)
				swap(S+(++P)*size, S+I*size, size);
			if((*cmp)(t, S+J*size) == 0)
				swap(S+(--Q)*size, S+J*size, size);
		}
		swap(S+l*size, S+J*size, size);

		I = J+1; J = J-1;
		for(k = l+1; k <= P; ++k)
			swap(S+k*size, S+(J--)*size, size);
		for(k = r; k >= Q; --k)
			swap(S+k*size, S+(I++)*size, size);
		s[++top].l = l; s[top].r = J;
		s[++top].l = I; s[top].r = r;
	}
	free(t);
	return;
}
