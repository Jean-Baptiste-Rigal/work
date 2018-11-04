#include <stdio.h>
#include <math.h>

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

unsigned cmp_count; // number of comparisons performed

int increasing(int a, int b)
{
   ++cmp_count;
   if (a < b)
     return -1;
   return a > b;
}

#define my_test(val) \
  cmp_count = 0; \
  printf("linear_search(a, %u, %d, increasing) = %u\n", \
         asize, (val), linear_search(a, asize, (val), increasing)); \
  printf("\twith %u comparisons\n", cmp_count);

int main()
{
  int a[] = { 1, 2, 3, 4, 5, 6, 9, 12, 15, 20, 25, 35, 38, 40, 41 };
  unsigned asize = sizeof(a) / sizeof(*a);
  puts("a[]:");
  print_int_array(stdout, a, asize);
  my_test(0);
  my_test(6);
  my_test(8);
  my_test(41);
  my_test(42);
  return 0;
}

