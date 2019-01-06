#ifndef _OVERSEER_H
#define _OVERSEER_H

#include "systemc.h"
#include "defines.h"

SC_MODULE(OVERSEER)
{
    sc_in<bool>  clk_i;
    sc_in<bool>  done;
    sc_out<bool> req_write;
    sc_out<bool> req_read;

    sc_in  <bool>    forward_done;
    sc_out <bool>   forward_start;
    sc_in  <bool>   backward_done;
    sc_out <bool>  backward_start;

    sc_out<bool> get_pat;

    SC_HAS_PROCESS(OVERSEER);

    OVERSEER(sc_module_name nm);
    ~OVERSEER();

    void mainThread();
private:

    void bus_write(int addr, float data);
    int  bus_read(int addr);

};


#endif
