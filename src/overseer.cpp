#include "overseer.h"

using namespace std;

OVERSEER::OVERSEER(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    addr_bo("addr_bo"),
    data_bi("data_bi"),
    data_bo("data_bo"),
    req_write("req_write"),
    req_read("req_read"),
    get_pat("get_pat"),
    pat_ready("pat_ready")
{
    addr_bo.initialize(0);
    data_bo.initialize(0);
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
    int set_size = SET_SIZE;

    for(int i = set_size; i > 0; i--)
    {
        bus_write(i, (i+1)*2);
    }

    for(int i = set_size; i > 0; i--)
    {
        bus_read(GENERATED);
        bus_read(REFERENCE);
    }

    sc_stop();
}

int OVERSEER::bus_read(int addr)
{
    int data;

    wait();
    addr_bo.write(addr);
    req_read.write(1);

    wait();
    req_read.write(0);

    wait();
    data = data_bi.read();
    get_pat.write(1);
    wait();
    get_pat.write(0);
    cout << "OVERSEER: READ " << endl;
    cout << "  -> addr: " << hex << addr << endl;
    cout << "  -> data: " << hex << data << endl;

    return data;

}

void OVERSEER::bus_write(int addr, int data)
{
    wait();
    addr_bo.write(addr);
    data_bo.write(data);
    req_write.write(1);
    
    wait();
    req_write.write(0);
    
    cout << "OVERSEER: WRITE " << endl;
    cout << "  -> addr: " << hex << addr << endl;
    cout << "  -> data: " << hex << data << endl;

}
