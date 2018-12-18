#ifndef _OVERSEER_H
#define _OVERSEER_H

#include "systemc.h"
#include "defines.h"

SC_MODULE(OVERSEER)
{
    sc_in<bool>  clk_i;
    sc_out<int>  addr_bo;
    sc_in<int>   data_bi;
    sc_out<int>  data_bo;
    sc_out<bool> req_write;
    sc_out<bool> req_read;

    sc_out<bool> get_pat;
    sc_in<bool> pat_ready;

    SC_HAS_PROCESS(OVERSEER);

    OVERSEER(sc_module_name nm);
    ~OVERSEER();

    void mainThread();
private:

    void bus_write(int addr, int data);
    int  bus_read(int addr);

};


#endif