#include <stdint.h>
#include <stdio.h>
#include <time.h>

int *sieve(int n);

int main() {
  clock_t start, finish; 
  double Total_time; 

  start = clock(); 

  int *primes = sieve(400);
  for (int *cur = primes, i = 0; *cur; cur++, i++) {
    printf("%8d", *cur);
    if (i % 8 == 7 || !*(cur + 1)) printf("\n");
  }

  finish = clock(); 
  Total_time = (double)(finish-start) / CLOCKS_PER_SEC; 
  printf( "%f seconds/n", Total_time); 
}
