#ifndef _LAYER
#define _LAYER

#include "systemc.h"
#include "defines.h"
#include <cstdlib>

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

    LAYER(sc_module_name nm, int curr, int prev, int next);
    ~LAYER();

    void compute();
    int curr;
    int prev;
    int next;
    int input_v_flag = 0;
    int output_v_flag = 0;
    int prev_got = 0;
    int counter = 0;
    int addr_buf = 0;
    int wait = 0;
    bool compute_flag = 0;
    bool was_request = 0;
    float** weights = new float*[next];
    float* input = new float[prev];
    float* output = new float[next];


private:
};

#endif