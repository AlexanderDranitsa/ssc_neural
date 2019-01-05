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
    int is_read = 0;
    int is_write = 0;
    for (int i = 0; i < LAYERS + 2; i++){
        //cout << "TRANSMIT" << i << "=" << in_req_write[i].read() << endl;
        if(in_req_read[i].read()){
            //cout << "BUS READ" << endl;
            is_read = 1;
            break;
        } else if (in_req_write[i].read()){
            //cout << "BUS WRITE" << endl;
            is_write = 1;
            break;
        }
    }
    if (is_read){
        out_req_read.write(1);
        out_req_write.write(0);
        is_read = 0;
    } else if (is_write){
        out_req_write.write(1);
        out_req_read.write(0);
        is_write = 0;
    } else {
        out_req_read.write(0);
        out_req_write.write(0);
    }
}