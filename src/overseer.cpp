#include "overseer.h"

using namespace std;

OVERSEER::OVERSEER(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    req_write("req_write"),
    req_read("req_read"),
    get_pat("get_pat")
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
    for(int i = 0; i < SET_SIZE; i++)
    {
        bus_write(i, shared_d);
    }

    for(int i = 0; i < SET_SIZE; i++)
    {
        bus_read(i);
    }

    shared_d = 123.456;

    for(int i = 0; i < SET_SIZE; i++)
    {
        bus_write(i, shared_d);
    }

    for(int i = 0; i < SET_SIZE; i++)
    {
        bus_read(i);
    }

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

    get_pat.write(1);
    wait();
    get_pat.write(0);
    cout << "OVERSEER: READ " << endl;
    cout << "  -> addr: " << shared_a << endl;
    cout << "  -> data: " << shared_d << endl;

    return data;
}

void OVERSEER::bus_write(int addr, float data)
{
    wait();
    shared_a = addr;
    shared_d = data;
    req_write.write(1);
    
    wait();
    req_write.write(0);
    
    cout << "OVERSEER: WRITE " << endl;
    cout << "  -> addr: " << shared_a << endl;
    cout << "  -> data: " << shared_d << endl;
}
