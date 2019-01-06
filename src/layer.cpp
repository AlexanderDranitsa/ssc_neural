#include "layer.h"

LAYER::LAYER(sc_module_name nm, int curr, int prev, int next)
    :sc_module(nm),
    curr(curr),
    prev(prev),
    next(next),
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
    counter = 0;
}

LAYER::~LAYER()
{
}

void LAYER::compute()
{
    prev_got = 0;
    read_req.write(0);
    write_req.write(0);
    forward_out.write(0);
    backward_out.write(0);
    if (counter == -1){
        forward_out.write(1);
        counter = 0;
    }
    if (was_request){
        if (shared_a == (0xf000 | addr_buf)){
            shared_a -= 0xf000;
            int index = (shared_a % 100);
            input[index] = shared_d;
            cout << "WROTE " << shared_d << " INTO " << shared_a << endl;
            was_request = 0;
            counter++;
        }
    }
    if (input_v_flag && (was_request == 0)){
        addr_buf = (curr * 1000) + counter;
        shared_a = addr_buf;
        read_req.write(1);
        was_request = 1;
        next_trigger();
        input[counter] = shared_d;
        if ( counter == prev - 1){
            counter = -1;
            input_v_flag = 0;
        }
    }
    if (output_v_flag){
        shared_a = (curr * 1000) + counter;
        read_req.write(1);
        was_request = 1;
        next_trigger();
        input[counter] = shared_d;
        if ( counter == prev - 1){
            counter = -1;
            input_v_flag = 0;
        } else {
            counter++;
        }
    } else if (forward_in.read()){
        cout << "FORWARD " << curr << endl;
        cout << prev << " " << curr << " " << next << endl;
        input_v_flag = 1;
    } else if (backward_in.read()){
        cout <<  "BACKWARD " << curr << endl;
    }
}