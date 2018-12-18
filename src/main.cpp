#include "overseer.h"
#include "mem.h"
#include "p_gen.h"

int sc_main(int argc, char* argv[])
{

    OVERSEER OVERSEER("OVERSEER");
    Mem memory("memory");
    P_GEN gen("P_GEN");

    sc_clock clk("clk", sc_time(10, SC_NS));
    sc_signal<int> addr;
    sc_signal<int> data_OVERSEER_bo;
    sc_signal<int> data_OVERSEER_bi;
    sc_signal<bool> wr;
    sc_signal<bool> rd;
    sc_vector< sc_signal<bool> > vec_bus("vec_bus", 49);

    sc_signal<bool> get_pat;
    sc_signal<bool> pat_ready;

    OVERSEER.clk_i(clk);
    OVERSEER.addr_bo(addr);
    OVERSEER.data_bi(data_OVERSEER_bi);
    OVERSEER.data_bo(data_OVERSEER_bo);
    OVERSEER.req_write(wr);
    OVERSEER.req_read(rd);

    OVERSEER.get_pat(get_pat);
    OVERSEER.pat_ready(pat_ready);

    gen.request(get_pat);
    gen.done(pat_ready);
    gen.clk_i(clk);
    gen.vector(vec_bus);

    memory.clk_i(clk);
    memory.addr_bi(addr);
    memory.data_bi(data_OVERSEER_bo);
    memory.data_bo(data_OVERSEER_bi);
    memory.bus_is_set(wr);
    memory.read_pending(rd);

    sc_trace_file *wf = sc_create_vcd_trace_file("wave");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, addr, "addr_bo");
    sc_trace(wf, data_OVERSEER_bi, "data_bi");
    sc_trace(wf, data_OVERSEER_bo, "data_bo");
    sc_trace(wf, wr, "wr");
    sc_trace(wf, rd, "rd");

    sc_start();
    sc_close_vcd_trace_file(wf);
    return(0);
}
