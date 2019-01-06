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
    SC_METHOD(compute);
    sensitive << clk_i.pos();

    srand(time(NULL));
    for (int i = 0; i < next; i++){
        weights[i] = new float[prev];
        for (int j = 0; j < prev; j++){
            weights[i][j] = ((rand() % 10) - 5) * 0.1;
        }
    }
    for (int i = 0; i < next; i++){
        for (int j = 0; j < prev; j++){
            //cout << weights[i][j] << " ";
        }
        cout << endl;
    }
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
    if (wait){
        wait--;
    } else {
        if (counter == 0xdead){
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
            addr_buf = 1000 + ((curr - 1) * 100) + counter;
            shared_a = addr_buf;
            read_req.write(1);
            write_req.write(0);
            was_request = 1;
            wait = 1;
            input[counter] = shared_d;
            if ( counter == prev){
                counter = -1;
                input_v_flag = 0;
                compute_flag = 1;
                wait = 1;
            }
        }
        if (compute_flag){
            for (int i = 0; i < next; i++){
                for (int j = 0; j < prev; j++){
                    output[i] += input[j] * weights[i][j];
                }
                //cout << output[i] << endl;
            }
            compute_flag = 0;
            output_v_flag = 1;
        }
        if (output_v_flag){
            if (counter < 0)
                counter = 0;
            write_req.write(1);
            read_req.write(0);
            shared_a = 1000 + (curr * 100) + counter;
            shared_d = output[counter];
            wait = 1;
            if ( counter == next - 1){
                counter = 0xdead;
                output_v_flag = 0;
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
}