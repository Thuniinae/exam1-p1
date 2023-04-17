#ifndef FILTER_H
#define FILTER_H

#include"param.h"
#include "ready_valid_port.h"

class FILTER:public sc_module{
public:
    SC_HAS_PROCESS(FILTER);
    FILTER(sc_module_name name){
        SC_THREAD(do_filter);
        sensitive << clk.pos();
        dont_initialize();
    }
    rdyvld_port_in<sc_ufixed_fast<53,10>> a, b;
    rdyvld_port_out<sc_ufixed_fast<53,10>> r;
    sc_in_clk clk;
private:
    sc_ufixed_fast<53,10>_a, _b, _r;

private:
    void do_filter(){
        a.rdy.write(false);
        b.rdy.write(false);
        r.vld.write(false);
        while (true){
            _a = a.read();
            cout << sc_time_stamp() << " receive " << _a << endl;
            _r = _a + _b;
            r.write(_r);
            wait();
        }
    }
        
};




#endif