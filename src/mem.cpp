#include "mem.h"

Mem::Mem(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    bus_is_set("bus_is_set"),
    read_pending("read_pending")
{
    SC_METHOD(bus_write);
    sensitive << clk_i.pos();
    SC_METHOD(bus_read);
    sensitive << clk_i.pos();

    for (int i = 0; i < (sizeof(vectors)/sizeof(vectors[0])); i++){
        if (i == 0){
            vectors[i] = new float [IMG_SIZE];
        } else if (i == LAYERS + 1){
            vectors[i] =  new float [OUTPUT_LENGTH];
        } else {
            //cout << config[i - 1] << endl;
            vectors[i] =  new float [config[i - 1]];
        }
    }

    for (int i = 1; i < LAYERS; i++){
        //cout << i - 1 << endl;
        backprop[i - 1] =  new float [config[i]];
        //cout << config[i] << endl;
    }
}

Mem::~Mem()
{
}

void Mem::bus_read()
{
    if(bus_is_set.read()){
        int index = (shared_a % 100);
        int layer = (shared_a % 1000) - index;
        int mode  = (shared_a % 10000) - layer;
        cout << "MEM: READ " << endl;
        cout << "  -> addr: " << shared_a << endl;
        cout << "  -> data: " << shared_d << endl;
        reference[0] = shared_d;
        // if (mode == 0){
        //     reference[index] = shared_d;
        // }
        // if (mode == 1){
        //     vectors[layer][index] = shared_d;
        // }
        // if (mode == 2){
        //     backprop[layer][index] = shared_d;
        // }
    }
}

void Mem::bus_write()
{
    if(read_pending.read()){
        int index = (shared_a % 100);
        int layer = (shared_a % 1000) - index;
        int mode  = (shared_a % 10000) - layer;
        // if (mode == 0){
        //     shared_d = reference[index];
        // }
        // if (mode == 1){
        //     shared_d = vectors[layer][index];
        // }
        // if (mode == 2){
        //     shared_d = backprop[layer][index];
        // }
        shared_d = reference[0];
        cout << "MEM: WRITE " << endl;
        cout << "  -> addr: " << shared_a << endl;
        cout << "  -> data: " << shared_d << endl;
    }
}