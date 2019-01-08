#include "layer.h"
#include <cmath>

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
            //cout << weights[i][j] << " ";
        }
        //cout << endl;
    }
    if (curr == LAYERS){
        ref = new float[LAYERS];
    } else {
        recieved_deltas = new float[next];
    }
}

LAYER::~LAYER()
{
}

inline float sygm(float x)
{
    float base = 2.71;
    return (1/(1 + (pow(base, -x))));
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
        if (forward_in.read()){
            //cout << "FORWARD " << curr << endl;
            //cout << prev << " " << curr << " " << next << endl;
            input_v_flag = 1;
            was_request = 0;
            wait = 3;
        }
        if (was_request){
            if (shared_a == (0xf000 | addr_buf)){
                shared_a -= 0xf000;
                int index = (shared_a % 100);
                input[index] = shared_d;
                //cout << "WROTE " << shared_d << " INTO " << shared_a << endl;
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
            //input[counter] = shared_d;
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
                output[i] = sygm(output[i]);
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
        }
        // ~~~ //
        if (backward_in.read()){
            //cout <<  "BACKWARD " << curr << endl;
            counter = 0;
            wait = 3;
            if (curr == LAYERS){
                b_last++;
            } else {
                //cout << "LAYER BWD" << curr <<" HERE" << endl;
                b_mid++;
            }
        }
        if (load_ref){
            //cout << "loading ref" << endl;
            ref[counter] = shared_d;
            if (counter < LAYERS - 1){
                counter++;
                shared_a = counter;
                read_req.write(1);
                write_req.write(0);
                wait = 1;
            } else {
                load_ref = 0;
                //cout << "REF? " <<ref[0] <<" " << ref[1]<<" "  << ref[2] << endl;
                cmp_last = 1;
            }
        }
        if (cmp_last){
            //cout << "LAYER " << curr << endl;
            for (int i = 0; i < next; i++){
                my_deltas[i] = (ref[i] - output[i]) * (1 - output[i]) * output[i];
                cout << my_deltas[i] << endl;
            }
            //cout << endl<< endl;
            for (int i = 0; i < prev; i++){
                float accum = 0;
                for (int j = 0; j < next; j++){
                    accum += my_deltas[j] * weights[j][i];
                }
                transmit_deltas[i] = accum;
                cout <<curr << " " << transmit_deltas[i] << endl;
            }
            for (int i = 0; i < next; i++){
                for (int j = 0; j < prev; j++){
                    //cout << "was " << weights[i][j] << endl;
                    weights[i][j] += lrn_coef * my_deltas[i] * input[j];
                    //cout << "now "<< weights[i][j] << endl;
                    //cout << "delta "<< lrn_coef * my_deltas[i] * input[j] << endl << endl;
                }
            }
            cmp_last = 0;
            counter = 0;
            wait = 1;
            write_transmit_last = 1;
        }
        if (write_transmit_last){
            shared_a = 2000 + (LAYERS - 2)*100 + counter;
            shared_d = transmit_deltas[counter];
            if (counter < prev){
                //cout << "THIS " << transmit_deltas[counter] << " " << shared_d <<endl;
                counter++;
                write_req.write(1);
                read_req.write(0);
                wait = 1;
            } else {
                write_transmit_last = 0;
                counter = 0;
                backward_out.write(1);
            }
        }
        if (write_transmit_mid){
            if (curr > 1){
                shared_a = 2000 + (curr -2)*100 + counter;
                shared_d = transmit_deltas[counter];
                if (counter < prev){
                    //cout << "THIS " << transmit_deltas[counter] << " " << shared_d <<endl;
                    counter++;
                    write_req.write(1);
                    read_req.write(0);
                    wait = 1;
                } else {
                    //cout << "~~~ BACK LAYER " << curr << "TRANSMIT OVER" << endl;
                    write_transmit_mid = 0;
                    counter = 0;
                    backward_out.write(1);
                }
            } else {
                write_transmit_mid = 0;
                counter = 0;
                backward_out.write(1);
            }
        }
        if (b_last){
            //cout << "LAST" << endl;
            load_ref = 1;
            shared_a = counter;
            read_req.write(1);
            write_req.write(0);
            wait = 1;
            b_last = 0;
        }
        //
        if (cmp_mid){
            cout << "LAYER " << curr << endl;
            for (int i = 0; i < next; i++){
                my_deltas[i] = recieved_deltas[i] * (1 - output[i]) * output[i];
                // cout << recieved_deltas[i] << " " << (1 - output[i]) * output[i] << endl;
                // cout << "my delta " << my_deltas[i] << endl;
            }
            //cout << endl<< endl;
            if (curr > 1){
                for (int i = 0; i < prev; i++){
                float accum = 0;
                for (int j = 0; j < next; j++){
                    accum += my_deltas[j] * weights[j][i];
                }
                transmit_deltas[i] = accum;
                cout <<curr << " " << transmit_deltas[i] << endl;
            }
            }
            for (int i = 0; i < next; i++){
                for (int j = 0; j < prev; j++){
                    // cout << "was " << weights[i][j] << endl;
                    weights[i][j] += lrn_coef * my_deltas[i] * input[j];
                    // cout << "now "<< weights[i][j] << endl;
                    // cout << "delta "<< lrn_coef * my_deltas[i] * input[j] << endl << endl;
                }
            }
            cmp_mid = 0;
            counter = 0;
            wait = 1;
            write_transmit_mid = 1;
        }
        //
        if (load_deltas){
            recieved_deltas[counter] = shared_d;
            // cout << "THIS IS "<< counter <<"RDELTA" << recieved_deltas[counter] << " " << shared_d <<endl;
            if (counter < next - 1){
                counter++;
                shared_a = 2000 + (curr - 1)*100 + counter;
                read_req.write(1);
                write_req.write(0);
                wait = 1;
            } else {
                load_deltas = 0;
                cmp_mid = 1;
                wait = 1;
            }
        }
        if (b_mid){
            //cout << curr << endl;
            b_mid = 0;
            load_deltas = 1;
            shared_a = 2000 + (curr - 1)*100;
            read_req.write(1);
            write_req.write(0);
            wait = 1;
        }
    }
}