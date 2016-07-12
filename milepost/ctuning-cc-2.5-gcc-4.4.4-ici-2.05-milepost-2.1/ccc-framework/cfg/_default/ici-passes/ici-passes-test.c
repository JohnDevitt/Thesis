#include <stdio.h>

void ici_passes_test(void)
{
  int x=0;
  int y=0;
  FILE* ff=NULL;
  float test[1024];
  
  ff=fopen("dummy", "r");
  
  for (x=0; x<1024; x++)
  {
    fscanf(ff, "%f", &test[x]);
  }

  fclose(ff);
  
  for (x=0; x<1024; x++)
  {
    test[x]*=2;
  }

  for (x=0; x<1024; x++)
  {
    printf("%u) %f\n", x, test[x]);
  }
}
