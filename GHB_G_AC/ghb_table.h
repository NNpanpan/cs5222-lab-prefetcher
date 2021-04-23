#include <stdio.h>

#include <deque>
#include <vector>

typedef long long int ulli;

class GHBEntry
{
  private:
    ulli _addr = 0;
    bool _valid = true;
    GHBEntry* next_same_addr_entry = nullptr;
    GHBEntry* prev_entry = nullptr;

  public:
    void set_is_valid(bool valid)
    {
      _valid = valid;
    }

    void set_addr(ulli addr)
    {
      _addr = addr;
    }

    void set_next_same_addr_entry(GHBEntry* e)
    {
      next_same_addr_entry = e;
    }

    void set_prev_entry(GHBEntry* e)
    {
      prev_entry = e;
    }

    GHBEntry* next()
    {
      return next_same_addr_entry;
    }

    GHBEntry* prev()
    {
      return prev_entry;
    }

    ulli addr()
    {
      return _addr;
    }

    bool is_valid()
    {
      return _valid;
    }
};

class GlobalHistoryBuffer
{
  private:
    std::deque<GHBEntry*> tbl;
    size_t tbl_size;
    size_t pf_degree;

  public:
    GlobalHistoryBuffer()
    {}

    ~GlobalHistoryBuffer()
    {}

    GlobalHistoryBuffer(int tbl_size, int pf_degree) :
    tbl_size(tbl_size),
    pf_degree(pf_degree)
    {}

    void _remove_entry()
    {
      tbl.back()->set_is_valid(false);
      tbl.pop_back();
    }

    GHBEntry* add_entry(ulli addr, GHBEntry* next_entry)
    {
      GHBEntry* new_entry = new GHBEntry();
      new_entry->set_addr(addr);
      new_entry->set_next_same_addr_entry(next_entry);

      if (tbl.size() > 0)
      {
        tbl[0]->set_prev_entry(new_entry);
        // printf("Set prev of %lld with addr %lld to %lld with addr %lld\n", 
        //   tbl[0], tbl[0]->addr(), tbl[0]->prev(), tbl[0]->prev()->addr());
      }
      else
      {
        printf("Empty buffer\n");
      }

      tbl.push_front(new_entry);
      if (tbl.size() > tbl_size)
      {
        _remove_entry();
      }

      // printf("Buffer: ");
      // for (int i = 0; i < tbl.size(); i++)
      // {
      //   printf("%lld with addr %lld and prev %lld, ", tbl[i], tbl[i]->prev());
      // }
      // printf("\n");

      // printf("New GHB entry: %lld, addr %lld\n\n", new_entry, new_entry->addr());

      return new_entry;
    }

    std::vector<ulli> get_prefetch_candidates(GHBEntry* entry)
    {
      std::vector<ulli> ret;

      if (entry == nullptr || !entry->is_valid())
      {
        return ret;
      }

      GHBEntry* cur_entry = entry->next();

      while (cur_entry != nullptr 
        && cur_entry->is_valid() 
        && ret.size() < pf_degree)
      {
        // printf("Prefetch candidate: cur_entry: %lld, addr: %lld, prev: %lld\n", cur_entry, cur_entry->addr(), cur_entry->prev());
        ret.push_back(
          cur_entry->prev()->addr()
        );

        cur_entry = cur_entry->next();
      }

      return ret;
    }
};