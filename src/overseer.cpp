#include "overseer.h"

using namespace std;

OVERSEER::OVERSEER(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    forward_done("forward_done"),
    forward_start("forward_start"),
    backward_done("backward_done"),
    backward_start("backward_start"),
    req_write("req_write"),
    req_read("req_read"),
    get_pat("get_pat"),
    done("done")
{
    req_write.initialize(0);
    req_read.initialize(0);
    get_pat.initialize(0);

    SC_CTHREAD(mainThread, clk_i.pos());
}

OVERSEER::~OVERSEER()
{
}

void OVERSEER::mainThread()
{
    for (int iters = 0; iters < 2; iters++){
        get_pat.write(1);
        wait();
        get_pat.write(0);
        
        while (done.read() == 0){
            wait();
        }
        wait();

        for(int i = 0; i < 3; i++)
        {
            bus_read(i);
            ref[i] = shared_d;
        }
        cout << ref[0] << ref[1] << ref[2] << endl;

        forward_start.write(1);
        wait();
        forward_start.write(0);

        while (forward_done.read() == 0){
            wait();
        }
        cout << "FROM OVERSEER FWD DONE " << endl<< endl<< endl;

        wait();

        for(int i = 0; i < 3; i++)
        {
            bus_read(1000 + LAYERS * 100 + i);
            out[i] = shared_d;
        }

        cout << out[0] << " " << out[1] << " " << out[2] << " " << endl;

        float max_ref[2] = {0, ref[0]};
        float max_out[2] = {0, out[0]};

        for (int i = 1; i < 3; i++){
            if (ref[i] > max_ref[1]){
                max_ref[0] = i;
                max_ref[1] = ref[i];
            }
            if (out[i] > max_out[1]){
                max_out[0] = i;
                max_out[1] = out[i];
            }
        }

        cout << max_ref[0] << " " << max_ref[1] << endl;
        cout << max_out[0] << " " << max_out[1] << endl;

        if (max_ref[0] != max_out[0]){
            cout << "MISTAKE, START BACKPROP" << endl;
            bad++;
            backward_start.write(1);
            wait();
            backward_start.write(0);

            while (backward_done.read() == 0){
                wait();
            }
            cout << "FROM OVERSEER BCKWRD DONE " << endl<< endl<< endl;
        } else {
            cout << "GOOD" << endl;
            good++;
        }
    }
    cout << "GOOD = " << good << endl;
    cout << "BAD = " << bad << endl;
    sc_stop();
}

int OVERSEER::bus_read(int addr)
{
    shared_a = addr;
    req_read.write(1);
    wait();
    req_read.write(0);
    wait();

    cout << "OVERSEER: READ " << endl;
    cout << "  -> addr: " << shared_a - 0xf000<< endl;
    cout << "  -> data: " << shared_d << endl;
}

void OVERSEER::bus_write(int addr, float data)
{
    shared_a = addr;
    shared_d = data;
    req_write.write(1);
    
    wait();
    req_write.write(0);
    
    cout << "OVERSEER: WRITE " << endl;
    cout << "  -> addr: " << shared_a << endl;
    cout << "  -> data: " << shared_d << endl;
}
