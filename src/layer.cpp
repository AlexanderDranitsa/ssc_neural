#include "layer.h"

LAYER::LAYER(sc_module_name nm, int number)
    :sc_module(nm),
    number(number),
    clk_i("clk_i"),
    forward_in("forward_in"),
    forward_out("forward_out"),
    backward_in("backward_in"),
    backward_out("backward_out"),
    read_req("read_req"),
    write_req("write_req")
{
    forward_out.initialize(0);
    backward_out.initialize(0);
    SC_METHOD(compute);
    sensitive << clk_i.pos();
}

LAYER::~LAYER()
{
}

void LAYER::compute()
{
    forward_out.write(0);
    backward_out.write(0);
    if (forward_in.read()){
        cout << "FORWARD " << number << endl;
        forward_out.write(1);
    } else if (backward_in.read()){
        cout <<  "BACKWARD " << number << endl;
        backward_out.write(1);
    }
}