#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

float * generator(int k);

float rand_reel(int max);

void tri_merge(int k, float* Bin1, float* Bin2);

void ajout_merge(float* Bout1, float* Bout2, int *curseur_Bout1, int *curseur_Bout2, float element, int k);

float* tri(float* bloc, int k);

void sort(int low, int high, float* a, float*b, int max);

void merging(int low, int mid, int high, float* a, float* b, int max);

void tri_parallele(int n, int k, float** blocs);

int main(int argc, char const *argv[])
{
	clock_t start;
	clock_t end;
	double duree;
	start = clock();

	srand(time(NULL));

	omp_set_num_threads(1);

	int k, n, i, j;

	k = 10000;

	n = 1000;

	float** blocs;

	blocs = (float**)malloc(n*sizeof(float*));

	for (i = 0; i < n; i++)
	{
		blocs[i] = generator(k);
	}

	tri_parallele(n,k,blocs);

	// for (i = 0; i < n; i++)
	// {
	// 	for (j = 0; j < k; j++){
	// 		printf("%d : %f\n",j,blocs[i][j]);
	// 	}
	// 	printf("\n");
	// }

	for (i = 0; i < n; i++)
	{
		free(blocs[i]);
	}
	free(blocs);

	end = clock();
	duree = (end - start) / (double)(CLOCKS_PER_SEC);

	printf("%lf\n",duree );
	
	return 0;
}

float * generator(int k){
	int i;
	float* bloc;
	bloc = (float*)malloc(k*sizeof(float));
	for (i = 0; i < k; ++i)
	{
		bloc[i] = rand_reel(100);
	}

	return bloc;
}

float rand_reel(int max){
	return (((float)rand())/((float)RAND_MAX)*max);
}

void tri_merge(int k, float* Bin1, float* Bin2){
	int i, j, l;

	float * tampon;

	tampon = (float*)malloc(2*k*sizeof(float));

	// for(i = 0, j = 0, l = 0; j < k && l < k; i++){
	// 	if(Bin1[j] <= Bin2[l])
	// 		tampon[i] = Bin1[j++];
	// 	else
	// 		tampon[i] = Bin2[l++];
	// }

	i = 0;
	j = 0;
	l = 0;

		while(j < k && l < k){
			if(Bin1[j] <= Bin2[l])
				tampon[i] = Bin1[j++];
			else
				tampon[i] = Bin2[l++];
		}

	while(j < k){
		tampon[i++] = Bin1[j++];
	}
	while(l < k){
		tampon[i++] = Bin2[l++];
	}

	for(i = 0; i < k; i++){
		Bin1[i] = tampon[i];
	}
	for(i = k; i < 2*k; i++){
		Bin2[i-k] = tampon[i];
	}
	free(tampon);
	
}

float* tri(float* bloc, int k){
	float* tampon;
	tampon = (float*)malloc(k*sizeof(float));
	sort(0, k-1, bloc, tampon, k-1);
	free(tampon);
	return bloc;
}

void merging(int low, int mid, int high, float* a, float* b, int max) {
   int l1, l2, i;

   l1 = low;
   l2 = mid+1;
   i = low;

	   while(l1 <= mid && l2 <= high){
	   	if(a[l1] <= a[l2])
	        b[i] = a[l1++];
	     else
	        b[i] = a[l2++];
	   	i++;
	   }

   
   while(l1 <= mid)    
      b[i++] = a[l1++];

   while(l2 <= high)   
      b[i++] = a[l2++];

   for(i = low; i <= high; i++)
      a[i] = b[i];
}

void sort(int low, int high, float* a, float*b, int max) {
   int mid;
   
   if(low < high) {
      mid = (low + high) / 2;

      sort(low, mid, a, b, max);
      sort(mid+1, high, a, b, max);
      merging(low, mid, high, a, b, max);
   } else { 
      return;
   }   
}

void tri_parallele(int n, int k, float** blocs){
	int  i, j, l, b1, b2, min, max;
	for (i = 0; i < n; ++i)
	{
		tri(blocs[i], k);
	}

	for (j = 0; j < n; j++)
	{
		l = 1+(j%2);
		for (i = 0; i < n/2; i++)
		{
			b1 = (l+2*i)%n;
			b2 = (l+2*i+1)%n;
			if(b1 < b2){
				min = b1;
				max = b2;
			}else{
				min = b2;
				max = b1;
			}
			tri_merge(k, blocs[min],blocs[max]);
		}
	}
}