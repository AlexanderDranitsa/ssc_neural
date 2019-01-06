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
    get_pat.write(1);
    wait();
    get_pat.write(0);
    
    while (done.read() == 0){
        wait();
    }
    wait();

    forward_start.write(1);
    wait();
    forward_start.write(0);

    while (forward_done.read() == 0){
        wait();
    }
    cout << "FROM OVERSEER FWD DONE " << endl<< endl<< endl;

    // backward_start.write(1);
    // wait();
    // backward_start.write(0);

    // while (backward_done.read() == 0){
    //     wait();
    // }
    // cout << "FROM OVERSEER BCKWRD DONE " << endl<< endl<< endl;
    // cout << "REFERENCE~~~~~~~~~~~~~~~~~~~~~" << endl<< endl<< endl;
    // for(int i = 0; i < 3; i++)
    // {
    //     bus_read(i);
    // }
    // cout << "VECTOR~~~~~~~~~~~~~~~~~~~~" << endl<< endl<< endl;
    // for(int i = 0; i < INPUT_LENGTH; i++)
    // {
    //     bus_read(1000 + i);
    // }

    sc_stop();
}

int OVERSEER::bus_read(int addr)
{
    float data;
    shared_a = addr;
    req_read.write(1);
    wait();
    req_read.write(0);

    wait();

    cout << "OVERSEER: READ " << endl;
    cout << "  -> addr: " << shared_a << endl;
    cout << "  -> data: " << shared_d << endl;

    return data;
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
