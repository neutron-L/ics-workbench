#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_prime[N];
static int  primes[N];

int *sieve(int n) {
  // assert(n + 1 < N);
  // for (int i = 0; i <= n; i++)
  //   is_prime[i] = true;


  // int *p = primes;
  // for (int i = 2; i <= n; i++) {
  //   if (is_prime[i])
  //   {
  //     *p++ = i;
  //     for (int j = i * i; j <= n; j += i) {
  //       is_prime[j] = false;
  //     }
  //   }
  // }
  // *p = 0;

  // return primes;
  assert(n+1<N);
  int tot=3;
  primes[0]=2;
  primes[1]=3;
  primes[2]=5;
	for(int i=6;i<=n;++i){
    if((i&1)==0) continue;
    if(i%3==0) continue;
    if(i%5==0) continue;
		if(!is_prime[i]) primes[tot++]=i;
		for(int j=3;j<tot && i*primes[j]<=n;++j){
			is_prime[i*primes[j]]=true;
			if(i%primes[j]==0) break;
		}
	}
  return primes;
}