#ifndef _MEM_H
#define _MEM_H

#include "systemc.h"
#include "defines.h"

SC_MODULE(Mem)
{
    sc_in<bool>  clk_i;
    sc_in<bool>  bus_is_set;
    sc_in<bool>  read_pending;

    SC_HAS_PROCESS(Mem);

    Mem(sc_module_name nm);
    ~Mem();

    void bus_write();
    void bus_read();

private:
    int config[LAYERS] = CONFIG;
    float* vectors[LAYERS + 2];
    float* backprop[LAYERS - 1];
    float reference[3];
};

#endif