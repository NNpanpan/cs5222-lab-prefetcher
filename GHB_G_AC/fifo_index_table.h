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
};

class IndexTable
{
  private:
    std::deque<IndexTableEntry> tbl;
    size_t tbl_size;
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

    void _remove_entry()
    {
      ulli to_rm_addr = tbl.back().addr();

      _cur_addresses.erase(to_rm_addr);
      tbl.pop_back();
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

      tbl.push_front(IndexTableEntry(addr, entry));
      _cur_addresses.insert(addr);

      if (tbl.size() > tbl_size) 
      {
        _remove_entry();
      }
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