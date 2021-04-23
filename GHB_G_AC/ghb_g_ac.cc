//
// Data Prefetching Championship Simulator 2
// Seth Pugsley, seth.h.pugsley@intel.com
//


#include <stdio.h>
#include "../inc/prefetcher.h"
// #include "fifo_index_table.h"
// #include "modulo_index_table.h"
#include "lru_index_table.h"

#include <vector>

#include <iostream>

typedef long long int ulli;

int TBL_ENTRIES = 8192;
int PREFETCH_DEGREE = 4;

GlobalHistoryBuffer global_history_bf;
IndexTable index_tbl;
ulli pf_cnt = 0;
ulli pf_warm = 0;
ulli lines_fetch = 0;
ulli successful_fetch = 0;
ulli counter = 0;

void l2_prefetcher_initialize(int cpu_num)
{
  // printf("No Prefetching\n");
  // you can inspect these knob values from your code to see which configuration you're runnig in
  // printf("Knobs visible from prefetcher: %d %d %d\n", knob_scramble_loads, knob_small_llc, knob_low_bandwidth);

  global_history_bf = GlobalHistoryBuffer(TBL_ENTRIES, PREFETCH_DEGREE);
  index_tbl = IndexTable(TBL_ENTRIES);
}

void l2_prefetcher_operate(int cpu_num, unsigned long long int addr, unsigned long long int ip, int cache_hit)
{
  // uncomment this line to see all the information available to make prefetch decisions
  //printf("(0x%llx 0x%llx %d %d %d) ", addr, ip, cache_hit, get_l2_read_queue_occupancy(0), get_l2_mshr_occupancy(0));

  counter++;

  if (cache_hit)
  {
    return;
  }

  // Add entry to GHB
  GHBEntry* ghb_ptr = index_tbl.get_ghb_ptr(addr);
  ghb_ptr = global_history_bf.add_entry(addr, ghb_ptr);

  // Add entry to Index Table
  index_tbl.add_entry(addr, ghb_ptr, counter);

  // Get prefetch candidates
  std::vector<ulli> to_pf = global_history_bf.get_prefetch_candidates(ghb_ptr);

  // Prefetch
  if (to_pf.size() == 0)
  {
    // std::cout << "No prefetching\n";
    return;
  }

  pf_cnt++;
  // std::cout << "Prefetching: ";
  for (ulli& a : to_pf)
  {
    // std::cout << a << " ";
    lines_fetch++;
    if (l2_prefetch_line(cpu_num, addr, a, FILL_L2))
    {
      successful_fetch++;
    }
  }
  // std::cout << std::endl;
}

void l2_cache_fill(int cpu_num, unsigned long long int addr, int set, int way, int prefetch, unsigned long long int evicted_addr)
{
  // uncomment this line to see the information available to you when there is a cache fill event
  //printf("0x%llx %d %d %d 0x%llx\n", addr, set, way, prefetch, evicted_addr);
}

void l2_prefetcher_heartbeat_stats(int cpu_num)
{
  // printf("Prefetcher heartbeat stats\n");
}

void l2_prefetcher_warmup_stats(int cpu_num)
{
  // printf("Prefetcher warmup complete stats\n");
  // printf("--Prefetched %lld times during warmup\n\n", pf_cnt);
  // pf_warm = pf_cnt;
}

void l2_prefetcher_final_stats(int cpu_num)
{
  printf("\nPrefetcher final stats\n");
  printf("--Prefetched attempts in total: %lld\n", pf_cnt);
  printf("--Prefetched lines:             %lld\n", lines_fetch);
  printf("--Prefetched successful:        %lld\n", successful_fetch);
}
