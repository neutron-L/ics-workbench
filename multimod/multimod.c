#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint64_t addmod(uint64_t a, uint64_t b, uint64_t m);
static uint64_t powermod(uint64_t a, uint64_t i, uint64_t m);
int highest_bit(uint64_t num);

// a * 2^i mod m
static uint64_t mod[64];
static bool vis[64];

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t res = 0;
  uint64_t i = 0;
  memset(mod, 0, sizeof(mod));
  memset(vis, 0, sizeof(vis));


  int h2 = highest_bit(m);

  // a mod m b mod m
  while (a >= m)
  {
    // a -= m;
    int h1 = highest_bit(a);
    if (a >= (m << (h1 - h2)))
      a -= m << (h1 - h2);
    else
      a -= m << (h1 - h2 - 1);
  }

  while (b >= m)
  {
    // a -= m;
    int h1 = highest_bit(b);
    if (b >= (m << (h1 - h2)))
      b -= m << (h1 - h2);
    else
      b -= m << (h1 - h2 - 1);
  }

  // replace a / b with m - a / b
  uint64_t ta = a, tb = b;
  a = (a <= (m>>1)) ? a : m - a;
  b = (b <= (m >> 1)) ? b : m - b;

  uint64_t qa = a, qb = b;
  
  while (b)
  {
    if (b & 1)
      res = addmod(res, powermod(a, i, m), m);
    b >>= 1;
    i++;
  }

  a = qa, b = qb;

  if ((a == ta && b != tb) || (a != ta && b == tb))
  {
    res = m - res;
    if (res == m) 
     res = 0;
  }


  return res;
}


static uint64_t powermod(uint64_t a, uint64_t i, uint64_t m)
{
  // a highest bit
  uint64_t t = a;
  uint64_t x = 0;
  uint64_t res = 0;

  if (vis[i])
    return mod[i];
  while (t)
  {
    t >>= 1;
    x++;
  }
  if (x + i > 64)
  {
    t = powermod(a, i - 1, m);
    res = addmod(t, t, m);
  }
  else
  {
    a <<= i;
    int h2 = highest_bit(m);
    while (a >= m)
    {
      // a -= m;
      int h1 = highest_bit(a);
      if (a >= (m << (h1 - h2)))
        a -= m << (h1 - h2);
      else
        a -= m << (h1 - h2 - 1);
    }
    res = a;
  }
  mod[i] = res;
  vis[i] = true;
  return res;
}

static uint64_t addmod(uint64_t a, uint64_t b, uint64_t m)
{
  uint64_t res = 0;
  int h2 = highest_bit(m);

  while (a >= m)
  {
    // a -= m;
    int h1 = highest_bit(a);
    if (a >= (m << (h1 - h2)))
      a -= m << (h1 - h2);
    else
      a -= m << (h1 - h2 - 1);
  }
  while (b >= m)
  {
    // a -= m;
    int h1 = highest_bit(b);
    if (b >= (m << (h1 - h2)))
      b -= m << (h1 - h2);
    else
      b -= m << (h1 - h2 - 1);
  }
  uint64_t sum = a + b;
  if (sum >= a)
  {
    while (sum >= m)
    {
      // sum -= m;
      int h1 = highest_bit(sum);
      if (sum >= (m << (h1 - h2)))
        sum -= m << (h1 - h2);
      else
        sum -= m << (h1 - h2 - 1);
    }
    res = sum;
  }
  else
  {
    res = addmod(sum + 1, (uint64_t)-1, m);
  }

  return res;
}


int highest_bit(uint64_t num)
{
	int l = 0, r = 64;
	int mid;
	while (l < r - 1)
	{
		mid = (l + r) / 2;
		if ((1ul<<mid) > num)
			r = mid;
		else if ((1ul<<mid) < num)
			l = mid;
		else
			return mid + 1;
	}
	if ((1ul<<l) <= num)
		return r;
	else
		return 0;
}
