#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions


// define the cache lines cache sets 
typedef struct
{
  bool valid;
  bool dirty;
  uint32_t tag;
  uint8_t data[BLOCK_SIZE];
} Line;

typedef struct 
{
  Line * lines;
  uint8_t header;
  uint8_t * next;
  uint8_t * pre;
} Set;


typedef struct 
{
  int8_t sidx;
  int8_t lidx;
} Res;

static uint8_t SET_WIDTH;
static uint8_t ASSOCIATIVITY_WIDTH;
static uint32_t SET_SIZE;
static Set * cache;

#define get_idx(addr) (((addr) >> (BLOCK_WIDTH)) & ((1<<SET_WIDTH) - 1))
#define get_tag(addr) ((addr) >> (BLOCK_WIDTH + SET_WIDTH))
#define block_offset(addr) ((addr) & (BLOCK_SIZE - 1))

Res cache_access(uintptr_t addr)
{
  // get the cache set index
  uint32_t idx = get_idx(addr);
  uint32_t tag = get_tag(addr);
  
  // judge whether cache hit
  Set s = cache[idx];
  Line * lines = s.lines;
  int i;
  for (i = 0; i < SET_SIZE; i++)
  {
    if (lines[i].valid && lines[i].tag == tag)
      break;
  }
  // cache hit
  if (i < SET_SIZE)
  {
    // adjust the lru record
    if (i == s.header)
      s.header++;
    else
    {
      uint8_t *next, *pre;
      next = s.next;
      pre = s.pre;
      next[pre[i]] = next[i];
      pre[next[i]] = pre[i];
      pre[i] = pre[s.header];
      next[pre[i]] = i;
      next[i] = s.header;
      pre[s.header] = i;
    }
  }
  // cache miss
  else
  {
    uintptr_t block_num;
    // write back
    if (lines[s.header].valid && lines[s.header].dirty)
    {
      block_num = (lines[s.header].tag << SET_WIDTH) | s.header;
      mem_write(block_num, lines[s.header].data);
    }
    // read the block
    block_num = addr >> BLOCK_WIDTH;
    mem_read(block_num, lines[s.header].data);
    i = s.header++;
    lines[i].valid = true;
    lines[i].dirty = false;
    lines[i].tag = tag;
  }
  Res res;
  res.sidx = idx;
  res.lidx = i;

  return res; 
}



uint32_t cache_read(uintptr_t addr) {
  Res res = cache_access(addr);

  uint8_t *p = (void *)cache[res.sidx].lines[res.lidx].data + block_offset(addr & ~0x3);
  uint32_t * q = p;
  
  return *q;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
    Res res = cache_access(addr);

    Line * lines = cache[res.sidx].lines;
    lines[res.lidx].dirty = true;
    uint8_t * p = (uint8_t*)lines[res.lidx].data;
    p += block_offset(addr & ~0x3);
    uint32_t * q = p;
    *q = (*q & ~wmask) | (data & wmask);
}

void init_cache(int total_size_width, int associativity_width) {
  SET_WIDTH =  total_size_width - associativity_width - BLOCK_WIDTH;
  SET_SIZE = 1 << ASSOCIATIVITY_WIDTH;
  ASSOCIATIVITY_WIDTH = associativity_width;
  assert(SET_WIDTH == 6);
  cache = (Set *)malloc(sizeof(Set) * exp2(SET_WIDTH));

  // initialize cache sets
  int n = exp2(SET_WIDTH);
  int m = exp2(ASSOCIATIVITY_WIDTH);
  for (int i = 0; i < n; i++)
  {
    cache[i].lines = (Line *)malloc(sizeof(Line) * m);
    cache[i].next = (uint8_t *)malloc(sizeof(uint8_t) * m);
    cache[i].pre = (uint8_t *)malloc(sizeof(uint8_t) * m);

    for (int j = 0; j < m; j++)
    {
      cache[i].next[j] = (j + 1) % m;
      cache[i].pre[j] = (j - 1 + m) % m;
    }
    cache[i].header = 0;

    Line * lines = cache[i].lines;
    for (int j = 0; j < m; j++)
      lines[j].valid = false;
  }
}

void display_statistic(void) {
}
