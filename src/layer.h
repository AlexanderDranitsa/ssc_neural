#ifndef _LAYER
#define _LAYER

#include "systemc.h"
#include "defines.h"

SC_MODULE(LAYER)
{
    sc_in  <bool>         clk_i;
    sc_in  <bool>    forward_in;
    sc_out <bool>   forward_out;
    sc_in  <bool>   backward_in;
    sc_out <bool>  backward_out;
    sc_out <bool>      read_req;
    sc_out <bool>     write_req;

    SC_HAS_PROCESS(LAYER);

    LAYER(sc_module_name nm, int number);
    ~LAYER();

    int number;

    void compute();

private:
};

#endif