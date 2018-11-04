#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
unsigned int_width(int i)
{
	if (i == 0)
		return 1;
	if (i > 0)
	{
		int a = 0;
		while(i != 0 )
		{
			i = i/10;
			a++;
		}
		return a;
	}

	if (i < 0)
	{
		int a = 1;
		while(i != 0)
		{
			i = i/10;
			a++;
		}
		return a;
	}
}

unsigned ints_width(const int* tab, unsigned count)
{
	unsigned int nbmax = int_width(tab[0]);
	for ( unsigned int a = 1 ; a < count ; a++)
	{
		if (int_width(tab[a]) > nbmax)
			nbmax = int_width(tab[a]);
	}
	return nbmax;
}

void print_int_array(FILE* out, const int* tab, unsigned count)
{
	unsigned int nbspacemax = ints_width(tab, count);
	int max_colum_width = 0;
	int max_index_width = int_width(count-1);
	
	for (unsigned int a = 0 ; a < count ; a++)
	{
		if (max_colum_width == 0)
		{
			int nbspace = max_index_width - int_width(a);
			fprintf(out,"%*s",nbspace,"");
			fprintf(out,"[%d]",a);
			max_colum_width += max_index_width+2;
		}	
		fprintf(out, "%*d",nbspacemax+1,tab[a]);
		max_colum_width += nbspacemax+1;
		if (max_colum_width +nbspacemax+1 > 80 && count-1 > a)
		{
			max_colum_width = 0;
			fprintf(out, "\n");
		}
	}
	fprintf(out,"\n");
}

unsigned linear_search(const int* tab, unsigned count, int val, int (*cmp)(int a, int b))
{
    if (count == 0)
            return count;
    unsigned int a;
    for (a = 0 ; a < count ; a++)
    {
        if (cmp(tab[a],val) != -1)
            break;
    }
    return a;
}
void insert_sort(int* tab, unsigned count)
{
	for (unsigned a = 1 ; a < count ; a++)
	{
		int cle = tab[a];
		int j  = a - 1;
		while (j >= 0 && tab[j] > cle)
		{
			tab[j+1] = tab[j];
			j = j-1;
		}
		tab[j+1] = cle;
	}
}

void insert_sort_cmp(int* tab, unsigned count, int (*cmp)(int a, int b))
{
	int cle;
	for (unsigned int a = 1 ; a < count ; a++)
	{
		cle = tab[a];
		int j  = a-1;
		while (j >= 0 && cmp(tab[j], cle) > 0)
		{
			tab[j+1] = tab[j];
			j = j-1;
		}
		tab[j+1] = cle;
	}
}


unsigned binary_search(int* tab, unsigned count, int val, int (*cmp)(int a, int b))
{

  unsigned int start = 0;
  unsigned int end = count;
  while (start < end)
  {
      int pos = (end + start)/2;
    
      unsigned int temp = cmp(tab[pos],val);

      if (temp == 1)
          end = pos;
      else if (temp == -1)
          start = pos + 1;
      else if (temp == 0)
          return pos;
  }
  return start;
}
void bs_insert_sort_cmp(int *tab, unsigned count, int (*cmp)(int a, int b))
{
    unsigned a;
    int value;

    for (unsigned int j = 0 ; j < count ; j++)
    {
        value = tab[j];
        a = binary_search(tab , j ,value, cmp);
        memmove(tab + a + 1,tab + a , (j - a)*sizeof(int));
        tab[a] = value;

    }
}

unsigned interpolate(int *tab, unsigned b, unsigned e, int val,int (*cmp)(int, int))
{
    if (cmp(val,tab[e]) == 1)
        return e;
    else if (cmp(val,tab[b]) == -1)
        return b;
    else
    {
        float min = tab[b];
        float max = tab[e];
    
        float coeffi = (max - min) / (e - b);
        float constante = max - (coeffi * e);

        float pos = (val - constante) / coeffi;
        return round(pos);
    }
}

unsigned interpolation_search(int *tab, unsigned count, int val,int (*cmp)(int a, int b))
{
  unsigned int start = 0;
  unsigned int end = count;
  while (start < end)
  {
      int pos = interpolate(tab , start , end - 1 ,val, cmp);
    
      unsigned int temp = cmp(tab[pos],val);

      if (temp == 1)
          end = pos;
      else if (temp == -1)
          start = pos + 1;
      else if (temp == 0)
          return pos;
  }
  return start;
}

void heapify(int* tab, unsigned pos, unsigned n)
{ 
    unsigned leftchild = 2 * pos + 1;
    unsigned rightchild = 2 * pos +2;
    unsigned g;

    if (leftchild < n && tab[leftchild] > tab[pos])
        g = leftchild;
    else
        g = pos;

    if (rightchild < n && tab[rightchild] > tab[g])
        g = rightchild;
    
    if (g != pos)
    {
        int temp = tab[pos];
        tab[pos] = tab[g];
        tab[g] = temp;
        heapify(tab , g , n);
    }
}
void make_heap(int* tab, unsigned n)
{
    for (int a = n/2 - 1 ; a >= 0; a--)
        heapify(tab , a , n);
}

bool check_heap(int* heap, unsigned n)
{ 
    int tabtemp[n];
    memmove(tabtemp , heap , n*sizeof(int));
    make_heap(tabtemp,n);
    if (memcmp(tabtemp, heap , n * sizeof(int)))
        return false;
    else
        return true;   
}

void pretty_print_heap(FILE* out, const int* v, unsigned n)
{
    int max_words = ints_width(v , n);
    int max_height = log2(n);
    int width_last_line = (max_words + 1)*(pow(2,max_height));
    unsigned index = 0;
    for (unsigned a = 0 ; a <= max_height ; a++)
    {
        int c = pow(2,a);
        int spaces = (width_last_line / c) - max_words;
        int start_space = spaces / 2;

        for (int b = 0 ; b < c ; b++)
        {
            if (b == 0)
                fprintf(out,"%*s",start_space,"");
            else
                fprintf(out,"%*s",spaces,"");
            fprintf(out,"%*d",max_words,v[index]);
            index++;

            if (index == n)
                break;
        }
        fprintf(out,"\n");
    }
}

int pop_heap(int* heap, unsigned* n)
{
    int root = heap[0];
    heap[0] = heap[*n - 1];
    *n = *n - 1;
    make_heap(heap, *n);
    return root;
}

void heap_sort(int* tab, unsigned n)
{
    make_heap(tab, n);
    for (unsigned a = n - 1 ; a > 0 ; a--)
    {
        int tmp = tab[0];
        tab[0] = tab[a];
        tab[a] = tmp;
        heapify(tab,0,a);
    }
}
void heapify_cmp(int* tab, unsigned pos, unsigned n, int (*cmp)(int a, int b))
{
    unsigned leftchild = 2 * pos + 1;
    unsigned rightchild = 2 * pos +2;
    unsigned g;

    if (leftchild < n && cmp(tab[leftchild],tab[pos]) == 1)
        g = leftchild;
    else
        g = pos;

    if (rightchild < n && cmp(tab[rightchild],tab[g]) == 1)
        g = rightchild;
    
    if (g != pos)
    {
        int temp = tab[pos];
        tab[pos] = tab[g];
        tab[g] = temp;
        heapify_cmp(tab , g , n,cmp);
    }
}

void make_heap_cmp(int* v, unsigned n, int (*cmp)(int a, int b))
{
    for (int a = n/2 - 1 ; a >= 0; a--)
    heapify_cmp(v , a , n, cmp);
}

bool check_heap_cmp(const int* tab, unsigned n, int (*cmp)(int a, int b))
{
    int tabtemp[n];
    memmove(tabtemp , tab , n*sizeof(int));
    make_heap_cmp(tabtemp,n,cmp);
    if (memcmp(tabtemp, tab , n * sizeof(int)))
        return false;
    else
        return true;
}

int pop_heap_cmp(int* heap, unsigned* n, int (*cmp)(int a, int b))
{
    int root = heap[0];
    heap[0] = heap[*n - 1];
    *n = *n - 1;
    make_heap_cmp(heap, *n,cmp);
    return root;
}

void heap_sort_cmp(int* tab, unsigned n, int (*cmp)(int a, int b))
{
    make_heap_cmp(tab, n,cmp);
    for (unsigned a = n - 1 ; a > 0 ; a--)
    {
        int tmp = tab[0];
        tab[0] = tab[a];
        tab[a] = tmp;
        heapify_cmp(tab,0,a,cmp);
    }
}


unsigned partition_cmp(int* tab, unsigned b, unsigned e,unsigned p, int (*cmp)(int a, int b))
{
    int x = tab[p];
    int i = b - 1;
    int j = e;

    while(1)
    {
        do
        {
            i++;
        } while (cmp(tab[i],x) == -1);
    
       do
       {
            j--;
       } while (cmp(tab[j],x) == 1);

        if (j <= i)
            return i + (i == b);

        int temp = tab[i];
        tab[i] = tab[j];
        tab[j] = temp;
    }
}

unsigned pivot_median3(const int* tab, unsigned l, unsigned r,int (*cmp)(int a, int b))
{
    int start = tab[l];
    int end = tab[r - 1];
    int medium = tab[l + (r - l)/2];
    
    int tabsort[3] = {start , medium , end};
    insert_sort_cmp(tabsort , 3 , cmp);

    if (tabsort[1] == start)
        return l;
    else if (tabsort[1] == end)
        return r - 1;
    else
        return l + (r - l)/2;

}

void quick_sort_cmp(int* tab, unsigned count,unsigned (*pivot)(const int* tab, unsigned l, unsigned r,int (*cmp)(int a, int b)),int (*cmp)(int a, int b))
{
    unsigned l = 0;
    unsigned r = count;
    unsigned p;
    if ((r - l) > 1)
    {
        unsigned pi = pivot(tab, l, r , cmp);
        p = partition_cmp(tab , l , r, pi , cmp);
        quick_sort_cmp(tab , p, pivot, cmp);
        quick_sort_cmp(tab + p , r - p, pivot, cmp);
    }
}
int increasing(int a, int b)
{
   if (a < b)
     return -1;
   return a > b;
}

int decreasing(int a, int b)
{
   if (a > b)
     return -1;
   return a < b;
}
unsigned pivot_first(const int* tab, unsigned l, unsigned r,
                     int (*cmp)(int a, int b))
{
  (void) tab;
  (void) r;
  (void) cmp;
  return l;
}
uintmax_t cmp_count;   // include <stdint.h>

int my_decreasing(int a, int b)
{
   ++cmp_count;
   if (a < b)
     return -1;
   return a > b;
}

#define mycheck(piv, tab )                                      \
  {                                                             \
    cmp_count = 0;                                              \
    clock_t before = clock();                                   \
    quick_sort_cmp(tab, tsize, pivot_##piv, my_decreasing);     \
    clock_t after = clock();                                    \
    printf(#piv " used %ju comparisons and %ju ticks\n",        \
           cmp_count, (uintmax_t) (after - before));            \
  }

unsigned pivot_rand(const int* tab, unsigned l, unsigned r,
                    int (*cmp)(int a, int b))
{
  (void) tab;
  return l + (rand() % (r - l));
}
int main()
{
 srand(0);
   const unsigned tsize = 500000;
   int* tab1 = malloc(tsize * sizeof(int));
   int* tab2 = malloc(tsize * sizeof(int));
   int* tab3 = malloc(tsize * sizeof(int));
   for (unsigned i = 0; i < tsize; ++i)
     tab3[i] = tab2[i] = tab1[i] = rand(); // = i;

  mycheck(first, tab1);
  mycheck(median3, tab2);
  mycheck(rand, tab3);
  return 0;
}
