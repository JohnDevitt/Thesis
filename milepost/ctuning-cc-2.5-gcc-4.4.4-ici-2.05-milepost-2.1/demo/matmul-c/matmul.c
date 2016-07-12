/*
 Copyright (C) 2000-2010 by Grigori G.Fursin

 http://fursin.net/research

 UNIDAPT Group
 http://unidapt.org
*/

#include <stdio.h>
#include <stdlib.h>

#define Q 16

void matmul(float* A, float* B, float* C, int N);

int main(int argc, char* argv[])
{
  FILE* fgg=NULL;
  long fgg1, fgg2;
  int N=0;
  float QQ[Q];
  int i=0;
  int j=0;
  int k=0;

  float* A;
  float* B;
  float* C;

  if (argc<3)
  {
    printf("Usage:\n");
    printf("  matmul <matrix dimension> <repetitions>\n");
    return 1;
  }

  N=atoi(argv[1]);
  fgg2=atol(argv[2]);

  printf("matrix dimension: %u\n", N);

  if ((fgg=fopen("data","rt"))==NULL)
  {
    fprintf(stderr,"\nError: Can't find data!\n");
    return 1;
  }

  for (i=0; i<Q; i++)
  {
    fscanf(fgg, "%f", &QQ[i]);
  }

  fclose(fgg);

  A=malloc(N*N*sizeof(float));
  B=malloc(N*N*sizeof(float));
  C=malloc(N*N*sizeof(float));

  k=0;
  for (i=0; i<N; i++)
  {
    for (j=0; j<N; j++)
    {
      A[j+i*N]=QQ[k++]*rand()/RAND_MAX;
      if (k>=Q) k=0;
      B[j+i*N]=QQ[k++]*rand()/RAND_MAX;
      if (k>=Q) k=0;
      C[j+i*N]=QQ[k++]*rand()/RAND_MAX;
      if (k>=Q) k=0;
    }
  }

  for (fgg1=0; fgg1<fgg2; fgg1++)
  {
    matmul(A,B,C,N);
  }

  for (i=0; i<N; i++)
  {
    printf("%u) %f %f %f\n", i, A[i*N+i], B[i*N+i], C[i*N+i]);
  }

  free(C);
  free(B);
  free(A);

  return 0;
}

void matmul(float* A, float* B, float* C, int N)
{
  int i,j,k;

  for (i=0; i<N; i++)
  {
/* Grigori remarked this line to enable cross-function data reuse just for testing
    A[i*N+j]=0; */
    for (j=0; j<N; j++)
    {
      for (k=0; k<N; k++)
      {
        A[i*N+j]=A[i*N+j]+B[i*N+k]*C[k*N+j];
      }
    }
  }
}
