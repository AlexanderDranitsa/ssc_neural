#include "layer.h"

LAYER::LAYER(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    // forward_in("forward_in"),
    // forward_out("forward_out"),
    // backward_in("backward_in"),
    // backward_out("backward_out"),
    read_req("read_req"),
    write_req("write_req")
{
    SC_METHOD(compute);
    sensitive << clk_i.pos();
    cout << "QWEREWR" << endl;
}

LAYER::~LAYER()
{
}

void LAYER::compute()
{
    //cout << "LAYER " << number << endl;
}