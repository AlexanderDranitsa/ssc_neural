#ifndef _BUS_H
#define _BUS_H

#include "systemc.h"
#include "defines.h"
#include <cstdlib>

SC_MODULE(BUS)
{
    sc_in<bool>  clk_i;

    //to mem interface
    sc_out<bool>   out_req_write;
    sc_out<bool>    out_req_read;

    // to others
    sc_vector< sc_in<bool> > in_req_write;
    sc_vector< sc_in<bool> > in_req_read;

    //sc_in<bool> in_req_write;
    //sc_in<bool> in_req_read;

    SC_HAS_PROCESS(BUS);

    BUS(sc_module_name nm);
    ~BUS();

private:
    void transmit();
};


#endif
