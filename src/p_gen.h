#ifndef _P_GEN
#define _P_GEN

#include "systemc.h"
#include "defines.h"
#include <cstdlib>

SC_MODULE(P_GEN)
{

    sc_in<bool>  clk_i;
    sc_in<bool>   request;
    sc_out<bool>  done;
    // sc_out<int>  data_bo;
    // sc_out<int>  addr_bo;
    // sc_in<bool>  bus_is_set;

    SC_HAS_PROCESS(P_GEN);

    P_GEN(sc_module_name nm);
    ~P_GEN();

    void vector_write();
    void harass();

private:
    const bool m_triangle[49] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0,
        0, 0, 1, 0, 1, 0, 0,
        0, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    const bool m_square[49] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 0,
        0, 1, 0, 0, 0, 1, 0,
        0, 1, 0, 0, 0, 1, 0,
        0, 1, 0, 0, 0, 1, 0,
        0, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    const bool m_round[49] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 0, 0,
        0, 1, 1, 0, 1, 1, 0,
        0, 1, 0, 0, 0, 1, 0,
        0, 1, 1, 0, 1, 1, 0,
        0, 0, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    int fig[49];
    int ref[3] = {0};
};

#endif