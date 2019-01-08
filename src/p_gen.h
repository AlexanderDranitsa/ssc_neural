#ifndef _P_GEN
#define _P_GEN

#include "systemc.h"
#include "defines.h"
#include <cstdlib>

SC_MODULE(P_GEN)
{
    sc_in  <bool>      clk_i;
    sc_in  <bool>    request;
    sc_out <bool>  req_write;
    sc_out <bool>       done;

    SC_HAS_PROCESS(P_GEN);

    P_GEN(sc_module_name nm);
    ~P_GEN();

    void gen();
    void bus_write();

private:
    int rnd = 0;
    int flag_v = 0;
    int flag_r = 0;
    int counter = 0;
    const int m_triangle[INPUT_LENGTH] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0,
        0, 0, 1, 0, 1, 0, 0,
        0, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    const int m_square[INPUT_LENGTH] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 0,
        0, 1, 0, 0, 0, 1, 0,
        0, 1, 0, 0, 0, 1, 0,
        0, 1, 0, 0, 0, 1, 0,
        0, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    const int m_round[INPUT_LENGTH] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 0, 0,
        0, 1, 1, 0, 1, 1, 0,
        0, 1, 0, 0, 0, 1, 0,
        0, 1, 1, 0, 1, 1, 0,
        0, 0, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    float fig[INPUT_LENGTH];
    float ref[3] = {0};
};

#endif