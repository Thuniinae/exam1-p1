// connections of modules using signals
// CHENG-HAN YU
// 2023/04/16

/* NOTES:
1. clock declaration: sc_clock clk("clk", period, SC_NS);
2. clock interface: sc_in_clk clk;
3. clock in sensitive list need positive edge: sensitive << clk.pos
*/

#include <systemc>
#include <iomanip> // for std::setw
#include "sysc/datatypes/fx/fx.h" // for sc_fixed
using namespace std;
using namespace sc_core; // for sc_start(), SC_MODULE, ...
using namespace sc_dt; // for system-c datatypes

#include "testbench.h"
#include "filter.h"
#include "param.h"
#include"ready_valid_port.h"

int sc_main(int argc, char *argv[])
{
    TESTBENCH testbench("testbench");
    FILTER filter("filter");


    sc_signal<sc_ufixed_fast<53,10>> a, b, r;
    sc_clock clk("clk", 1, SC_NS);
    sc_signal<bool> a_rdy, a_vld, r_rdy, r_vld, b_vld, b_rdy;

    testbench.a.msg(a);
    filter.a.msg(a);
    testbench.b.msg(b);
    filter.b.msg(b);
    testbench.r.msg(r);
    filter.r.msg(r);
    testbench.a.rdy(a_rdy);
    filter.a.rdy(a_rdy);
    testbench.b.rdy(b_rdy);
    filter.b.rdy(b_rdy);
    testbench.r.rdy(r_rdy);
    filter.r.rdy(r_rdy);
    testbench.a.vld(a_vld);
    filter.a.vld(a_vld);
    testbench.b.vld(b_vld);
    filter.b.vld(b_vld);
    testbench.r.vld(r_vld);
    filter.r.vld(r_vld);

    testbench.clk(clk);
    filter.clk(clk);
    sc_start(1, SC_SEC);
    return 0;
}