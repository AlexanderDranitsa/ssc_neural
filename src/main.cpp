#include "overseer.h"
#include "mem.h"
#include "p_gen.h"
#include "defines.h"
#include "bus.h"
#include "layer.h"

float shared_d;
int shared_a;

int sc_main(int argc, char* argv[])
{
    OVERSEER OVERSEER("OVERSEER");
    Mem memory("memory");
    P_GEN P_GEN("P_GEN");
    BUS BUS("BUS");

    sc_clock clk("clk", sc_time(10, SC_NS));
    sc_vector< sc_signal<bool> > read_bus("read_bus",  LAYERS + 2);
    sc_vector< sc_signal<bool> > write_bus("write_bus", LAYERS + 2);

    sc_vector< sc_signal<bool> > forward_bus("forward_bus",  LAYERS - 1 );
    sc_vector< sc_signal<bool> > backprop_bus("backprop_bus", LAYERS - 1 );

    sc_signal<bool> get_pat;
    sc_signal<bool> done;

    sc_signal<bool> forward_start;
    sc_signal<bool> forward_done;

    sc_signal<bool> backward_start;
    sc_signal<bool> backward_done;

    sc_signal<bool> bus_to_mem_read_req;
    sc_signal<bool> bus_to_mem_write_req;

    OVERSEER.clk_i(clk);
    OVERSEER.req_write(write_bus[0]);
    OVERSEER.req_read(read_bus[0]);

    BUS.clk_i(clk);
    BUS.out_req_write(bus_to_mem_write_req);
    BUS.out_req_read(bus_to_mem_read_req);
    BUS.in_req_read(read_bus);
    BUS.in_req_write(write_bus);

    OVERSEER.get_pat(get_pat);
    OVERSEER.done(done);
    OVERSEER.forward_done(forward_done);
    OVERSEER.forward_start(forward_start);
    OVERSEER.backward_done(backward_done);
    OVERSEER.backward_start(backward_start);

    P_GEN.done(done);
    P_GEN.request(get_pat);
    P_GEN.clk_i(clk);
    P_GEN.req_write(write_bus[1]);

    memory.clk_i(clk);
    memory.bus_is_set(bus_to_mem_write_req);
    memory.read_pending(bus_to_mem_read_req);

    LAYER* layers_arr[LAYERS];

    int cfg[LAYERS] = CONFIG;
    for (int prev, next, i = 0; i < LAYERS; i++){
        if (i == 0){
            prev = INPUT_LENGTH;
        } else {
            prev = cfg[i - 1];
        }
        if (i == LAYERS - 1){
            next = 3;
        } else {
            next = cfg[i];
        }
        layers_arr[i] = new LAYER("layer", i + 1, prev, next);
        cout << "CREATED " << i + 1 << " " << prev << " " << next << endl;
        layers_arr[i]->clk_i(clk);
        layers_arr[i]->write_req(write_bus[i+2]);
        layers_arr[i]->read_req(read_bus[i+2]);
        // FWD
        if (i == 0){
            layers_arr[i]->forward_in(forward_start);
            layers_arr[i]->forward_out(forward_bus[0]);
        } else if (i == LAYERS - 1){
            layers_arr[i]->forward_out(forward_done);
            layers_arr[i]->forward_in(forward_bus[LAYERS - 2]);
        } else {
            layers_arr[i]->forward_out(forward_bus[i]);
            layers_arr[i]->forward_in(forward_bus[i - 1]);
        }
        if (i == 0){
            layers_arr[i]->backward_in(backprop_bus[0]);
            layers_arr[i]->backward_out(backward_done);
        } else if (i == LAYERS - 1){
            layers_arr[i]->backward_out(backprop_bus[i - 1]);
            layers_arr[i]->backward_in(backward_start);
        } else {
            layers_arr[i]->backward_out(backprop_bus[i - 1]);
            layers_arr[i]->backward_in(backprop_bus[i]);
        }
    }

    sc_trace_file *wf = sc_create_vcd_trace_file("wave");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, shared_a, "data_bi");
    sc_trace(wf, shared_d, "data_bo");
    sc_trace(wf, bus_to_mem_read_req, "read_bus[0]");
    sc_trace(wf, bus_to_mem_write_req, "write_bus[0]");

    sc_start();
    sc_close_vcd_trace_file(wf);
    return(0);
}
