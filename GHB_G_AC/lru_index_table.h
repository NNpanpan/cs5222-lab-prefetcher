#include <set>

#include "ghb_table.h"

typedef long long int ulli;

class IndexTableEntry
{
  private:
    ulli _addr;
    GHBEntry* _entry;
    ulli _last_used;

  public:
    IndexTableEntry(ulli addr, GHBEntry* ghb_entry, ulli last_used) :
    _addr(addr),
    _entry(ghb_entry),
    _last_used(last_used)
    {}

    ulli addr()
    {
      return _addr;
    }

    GHBEntry* ghb_entry()
    {
      return _entry;
    }

    void set_ghb_entry(GHBEntry* ghb_entry)
    {
      _entry = ghb_entry;
    }

    void set_last_used(ulli l_u)
    {
      _last_used = l_u;
    }

    void set_addr(ulli addr)
    {
      _addr = addr;
    }

    ulli last_used()
    {
      return _last_used;
    }
};

class IndexTable
{
  private:
    std::vector<IndexTableEntry> tbl;
    int tbl_size;
    std::set<ulli> _cur_addresses;

  public:
    IndexTable()
    {}

    ~IndexTable()
    {}

    IndexTable(int tbl_size) :
    tbl_size(tbl_size)
    {}

    bool has_addr(ulli addr)
    {
      return _cur_addresses.find(addr) != _cur_addresses.end();
    }

    void _replace_entry(ulli addr, GHBEntry* entry, ulli l_u)
    {
      ulli to_rm_addr = tbl[0].addr();
      int to_rm_index = 0;
      ulli to_rm_l_u = tbl[0].last_used();

      // Look for entry that was least recently used
      for (int i = 0; i < tbl_size; i++)
      {
        if (tbl[i].last_used() < to_rm_l_u)
        {
          to_rm_l_u = tbl[i].last_used();
          to_rm_addr = tbl[i].addr();
          to_rm_index = i;
        }
      }

      tbl[to_rm_index].set_ghb_entry(entry);
      tbl[to_rm_index].set_addr(addr);
      tbl[to_rm_index].set_last_used(l_u);
      _cur_addresses.erase(to_rm_addr);
    }

    void add_entry(ulli addr, GHBEntry* entry, ulli l_u)
    {
      if (has_addr(addr))
      {
        for (IndexTableEntry& e : tbl)
        {
          if (e.addr() == addr)
          {
            e.set_ghb_entry(entry);
            e.set_last_used(l_u);
            return;
          }
        }
      } 

      _cur_addresses.insert(addr);
      
      if (tbl.size() < (size_t) tbl_size)
      {
        tbl.push_back(IndexTableEntry(addr, entry, l_u));
        return;
      }

      _replace_entry(addr, entry, l_u);
    }

    GHBEntry* get_ghb_ptr(ulli addr)
    {
      if (!has_addr(addr))
      {
        return nullptr;
      }

      GHBEntry* ret_ptr = nullptr;

      for (IndexTableEntry& e : tbl)
      {
        if (e.addr() == addr)
        {
          ret_ptr = e.ghb_entry();
        }
      }

      return ret_ptr;
    }
};