#include "mem.h"

Mem::Mem(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    addr_bi("addr_bi"),
    data_bi("data_bi"),
    data_bo("data_bo"),
    bus_is_set("bus_is_set"),
    read_pending("read_pending")
{
    data_bo.initialize(0);

    SC_METHOD(bus_write);
    sensitive << clk_i.pos();

    SC_METHOD(bus_read);
    sensitive << clk_i.pos();
}

Mem::~Mem()
{
}

void Mem::bus_read()
{
    if(bus_is_set.read())
        ipnut[addr_bi.read()] = data_bi.read();
}

void Mem::bus_write()
{
    if(read_pending.read()){
        switch(addr_bi.read()){
            case GENERATED:
                data_bo.write(0xdead);
                break;
            case REFERENCE:
                data_bo.write(0xc0de);
                break;
        }
    }
}