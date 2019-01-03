#include "bus.h"

BUS::BUS(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    out_req_write("out_req_write"),
    out_req_read("out_req_read"),
    in_req_write("in_req_write", LAYERS + 2),
    in_req_read("in_req_read", LAYERS + 2)
{
    out_req_write.initialize(0);
    out_req_read.initialize(0);
    SC_METHOD(transmit);
    sensitive << clk_i.pos();
}

BUS::~BUS()
{
}

void BUS::transmit()
{
    if (in_req_read[0].read()){
        out_req_read.write(1);
        out_req_write.write(0);
    } else if (in_req_write[0].read()){
        out_req_write.write(1);
        out_req_read.write(0);
    } else {
        out_req_read.write(0);
        out_req_write.write(0);
    }
}