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

    for (int i = 0; i < LAYERS - 1; i++){
        backprop[i] =  new float [config[i]];
        //cout << "BP #" <<i << " SIZEOF = " << config[i] << endl;
    }
}

Mem::~Mem()
{
}

void Mem::bus_read()
{
    if(bus_is_set.read()){
        int index = (shared_a % 100);
        int layer = (shared_a / 100) % 10;
        int mode  = (shared_a / 1000) % 10;
        // cout << "~MEM: READ INTO " << endl;
        // cout << "  -> addr: " << shared_a << endl;
        // cout << "  -> data: " << shared_d << endl;
        if (mode == 0){
            //cout << "REF" << endl;
            reference[index] = shared_d;
        }
        if (mode == 1){
            //cout << "VEC" << endl;
            vectors[layer][index] = shared_d;
        }
        if (mode == 2){
            //cout << "BP" << endl;
            backprop[layer][index] = shared_d;
        }
    }
}

void Mem::bus_write()
{
    if(read_pending.read()){
        int index = (shared_a % 100);
        int layer = (shared_a / 100) % 10;
        int mode  = (shared_a / 1000) % 10;
        if (mode == 0){
            //cout << "REF" << endl;
            shared_d = reference[index];
        }
        if (mode == 1){
            //cout << "VEC" << endl;
            shared_d = vectors[layer][index];
        }
        if (mode == 2){
            //cout << "BP" << endl;
            shared_d = backprop[layer][index];
        }
        shared_a |= 0xf000;
        // cout << "~MEM: WRITE OUT" << endl;
        // cout << "  -> addr: " << shared_a - 0xf000<< endl;
        // cout << "  -> data: " << shared_d << endl;
    }
}