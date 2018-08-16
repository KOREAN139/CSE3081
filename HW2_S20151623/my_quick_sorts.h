#define MAX_STACK_SIZE 4096
#define MAX_BUF_SIZE 512

typedef struct{
	unsigned int score;
	float data[3];
	char comments[16];
} ELEMENT;

typedef struct{
	size_t l, r;
} stack;

typedef int _Cmpfun(const void *, const void *);

void qsort_orig(void *, size_t, size_t, _Cmpfun *);
void qsort_median_insert(void *, size_t, size_t, _Cmpfun *);
void qsort_median_insert_iter(void *, size_t, size_t, _Cmpfun *);
void qsort_final(void *, size_t, size_t, _Cmpfun *);
void insertion_sort(void *, size_t, size_t, _Cmpfun *);
size_t partition(void *, size_t, size_t, _Cmpfun, size_t);
void swap(char *, char *, size_t);
size_t median(size_t, size_t);
size_t median3(size_t, size_t, size_t);
