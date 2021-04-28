//
// Data Prefetching Championship Simulator 2
// Seth Pugsley, seth.h.pugsley@intel.com
//

// This implements the GHB PC/DC prefetcher

#include <stdio.h>
#include "./inc/prefetcher.h"

#include <iostream>
#include <vector>

typedef unsigned long long int ulli;
typedef std::pair<int, int> pii;

struct ghb_entry
{
  ulli address = -1;
  ghb_entry* next_ptr = nullptr;
  ghb_entry* prev_ptr = nullptr;
  ghb_entry* closest_same_pc_ptr = nullptr;
  bool valid = false;
};

struct it_entry
{
  ulli ip = -1;
  ghb_entry* ghb_ptr = nullptr;
};

struct ght
{
  int max_size = 256;
  int size = 0;
  ghb_entry* end_entry = nullptr;
  ghb_entry* start_entry = nullptr;
  std::vector<ghb_entry> entries;
};

struct it
{
  int max_size = 256;
  std::vector<it_entry> entries;
};

it index_table;
ght global_history_table;

ulli prev_addr1;
ulli prev_addr2;

int prefetch_degree;

void l2_prefetcher_initialize(int cpu_num)
{
  printf("No Prefetching\n");
  // you can inspect these knob values from your code to see which configuration you're runnig in
  printf("Knobs visible from prefetcher: %d %d %d\n", knob_scramble_loads, knob_small_llc, knob_low_bandwidth);

  for (int i = 0; i < index_table.max_size; i++)
  {
    it_entry entry;
    index_table.entries.push_back(entry);
  }

  prev_addr1 = prev_addr2 = -1;

  prefetch_degree = 1;
}

void add_entry(ulli addr, ulli ip)
{
  // Add new entry to
  it_entry* index_entry = &index_table.entries[ip % index_table.max_size];
  
  if (index_entry->ip != ip)
  {
    index_entry->ip = ip;
    index_entry->ghb_ptr = nullptr;
  }

  ghb_entry entry;
  entry.address = addr;
  entry.closest_same_pc_ptr = index_entry->ghb_ptr;

  entry.next_ptr = global_history_table.start_entry;
  std::cout << "-init entry okay\n";
  if (global_history_table.end_entry != nullptr)
  { // Update end_entry
    printf("--Summary of new entry: %lld, %lld, %lld, %lld, %lld, %d\n", 
      &entry, entry.address, entry.closest_same_pc_ptr, entry.next_ptr, entry.prev_ptr, entry.valid);
    global_history_table.end_entry->valid = false;
    ghb_entry* last_entry = global_history_table.end_entry;
    printf("--last_entry: %lld, %lld, %lld, %lld, %lld, %d\n", 
      last_entry, last_entry->address, last_entry->closest_same_pc_ptr, 
      last_entry->next_ptr, last_entry->prev_ptr, last_entry->valid);

    ghb_entry* second_last_entry = global_history_table.end_entry->prev_ptr;
    printf("--second_last_entry: %lld, %lld, %lld, %lld, %lld, %d\n", 
      second_last_entry, second_last_entry->address, second_last_entry->closest_same_pc_ptr, 
      second_last_entry->next_ptr, second_last_entry->prev_ptr, second_last_entry->valid);
    second_last_entry->next_ptr = &entry;
    global_history_table.end_entry = second_last_entry;
  }
  std::cout << "-modified end_entry\n";
  entry.prev_ptr = global_history_table.end_entry;

  if (global_history_table.size == 0) 
  {
    global_history_table.size = 1;
    global_history_table.start_entry = global_history_table.end_entry = &entry;
    entry.next_ptr = entry.prev_ptr = &entry;
  }
  else
  { // Update start_entry
    global_history_table.start_entry->prev_ptr = &entry;
    if (global_history_table.size == global_history_table.max_size)
    {
      global_history_table.entries.pop_back();
    }
  }

  index_entry->ghb_ptr = &entry;
  entry.valid = true;
  global_history_table.entries.push_back(entry);

  entry = *(global_history_table.start_entry);
  printf("--Summary of new entry: %lld, %lld, %lld, %lld, %lld, %d\n", 
      &entry, entry.address, entry.closest_same_pc_ptr, entry.next_ptr, entry.prev_ptr, entry.valid);
  // Done adding entry
}

void l2_prefetcher_operate(int cpu_num, ulli addr, ulli ip, int cache_hit)
{
  // uncomment this line to see all the information available to make prefetch decisions
  //printf("(0x%llx 0x%llx %d %d %d) ", addr, ip, cache_hit, get_l2_read_queue_occupancy(0), get_l2_mshr_occupancy(0));

  /*
  0. Add new entry to gh table, update index table
  1. Use ip, prev_addr1, prev_addr2 to calculate the latest delta pair -> delta_pair
  2. Use ip to search for ghb entry -> E
  3. Traverse from E, calculate _delta_pair.
    If same as delta_pair, traverse backward upto prefetch_degree, accumulate addresses using deltas
  4. Prefetch
  */
  
  add_entry(addr, ip);

  // Calculate delta_pair
  pii delta_pair;
  if (prev_addr1 >= 0 && prev_addr2 >= 0)
  {
    delta_pair = pii(addr - prev_addr1, prev_addr1 - prev_addr2);
  }
  else
  {
    return; // don't do anything since there's no delta_pair
  }

  // Fetch ghb_entry
  // Should skip the first block
  ghb_entry* latest_entry = index_table.entries[ip % index_table.max_size].ghb_ptr;
  ghb_entry* entry_to_use = latest_entry->closest_same_pc_ptr;

  if (entry_to_use == nullptr || !entry_to_use->valid)
  {
    return;
  }


  // Traverse entry_to_use and calculate _delta_pair
  bool sanity_check = entry_to_use->next_ptr != global_history_table.end_entry 
    && entry_to_use->next_ptr->next_ptr != global_history_table.end_entry;
  if (!sanity_check)
  { // No previous addresses to use
    return;
  }

  while (sanity_check)
  {
    int d1 = entry_to_use->address - entry_to_use->next_ptr->address;
    int d2 = entry_to_use->next_ptr->address - entry_to_use->next_ptr->next_ptr->address;

    if (d1 == delta_pair.first && d2 == delta_pair.second)
    {
      ulli accumulated_addr = addr;
      for (int i = 0; i < prefetch_degree; i++)
      {    
        accumulated_addr += entry_to_use->prev_ptr->address - entry_to_use->address;
        entry_to_use = entry_to_use->prev_ptr;

        l2_prefetch_line(cpu_num, addr, accumulated_addr, FILL_L2);
      }
    }
  }

}

void l2_cache_fill(int cpu_num, ulli addr, int set, int way, int prefetch, ulli evicted_addr)
{
  // uncomment this line to see the information available to you when there is a cache fill event
  //printf("0x%llx %d %d %d 0x%llx\n", addr, set, way, prefetch, evicted_addr);
}

void l2_prefetcher_heartbeat_stats(int cpu_num)
{
  printf("Prefetcher heartbeat stats\n");
}

void l2_prefetcher_warmup_stats(int cpu_num)
{
  printf("Prefetcher warmup complete stats\n\n");
}

void l2_prefetcher_final_stats(int cpu_num)
{
  printf("Prefetcher final stats\n");
}
