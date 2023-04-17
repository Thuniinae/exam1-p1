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
    sc_ufixed_fast<53,10> buffer[3];


private:
    void do_filter(){
        buffer[0] = 0;
        buffer[1] = a.read();
        buffer[2] = a.read();
        a.rdy.write(false);
        b.rdy.write(false);
        r.vld.write(false);
        for(int i = 0; i < 63; i++){
            _r = buffer[2]*1/2 + buffer[1]*1/3 + buffer[0]*1/6;
            wait(3, SC_NS);
            r.write(_r);
            buffer[0] = buffer[2];
            buffer[1] = a.read();
            buffer[2] = a.read();
        }
        _r = buffer[2]*1/2 + buffer[1]*1/3 + buffer[0]*1/6;
        wait(3, SC_NS);
        r.write(_r);
    }
};




#endif