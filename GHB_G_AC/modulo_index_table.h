#include <set>

#include "ghb_table.h"

typedef long long int ulli;

class IndexTableEntry
{
  private:
    ulli _addr;
    GHBEntry* _entry;

  public:
    IndexTableEntry(ulli addr, GHBEntry* ghb_entry) :
    _addr(addr),
    _entry(ghb_entry)
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

    void set_addr(ulli addr)
    {
      _addr = addr;
    }
};

class IndexTable
{
  private:
    std::vector<IndexTableEntry> tbl;
    size_t tbl_size;
    std::set<ulli> _cur_addresses;

  public:
    IndexTable()
    {}

    ~IndexTable()
    {}

    IndexTable(int tbl_size) :
    tbl_size(tbl_size)
    {
      tbl = std::vector<IndexTableEntry> (tbl_size, IndexTableEntry(-1, nullptr));
    }

    bool has_addr(ulli addr)
    {
      return tbl[addr % tbl_size].addr() == addr;
    }

    void add_entry(ulli addr, GHBEntry* entry)
    {
      if (has_addr(addr))
      {
        for (IndexTableEntry& e : tbl)
        {
          if (e.addr() == addr)
          {
            e.set_ghb_entry(entry);
            return;
          }
        }
      }

      int ind = addr % tbl_size;
      tbl[ind].set_addr(addr);
      tbl[ind].set_ghb_entry(entry);
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