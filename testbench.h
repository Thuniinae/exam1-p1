#ifndef TESTBENCH_H
#define TESTBENCH_H

#include <systemc>
#include <iomanip> // for std::setw
using namespace std;
using namespace sc_core; // for sc_start(), SC_MODULE, ...
using namespace sc_dt; // for system-c datatypes

#include"param.h"
#include"ready_valid_port.h"

const float x_input_signal[128]={0.500,   0.525,   0.549,   0.574,   0.598,   0.622,   0.646,   0.670,
                            0.693,   0.715,   0.737,   0.759,   0.780,   0.800,   0.819,   0.838,
                            0.856,   0.873,   0.889,   0.904,   0.918,   0.931,   0.943,   0.954,
                            0.964,   0.972,   0.980,   0.986,   0.991,   0.995,   0.998,   1.000,
                            1.000,   0.999,   0.997,   0.994,   0.989,   0.983,   0.976,   0.968,
                            0.959,   0.949,   0.937,   0.925,   0.911,   0.896,   0.881,   0.864,
                            0.847,   0.829,   0.810,   0.790,   0.769,   0.748,   0.726,   0.704,
                            0.681,   0.658,   0.634,   0.610,   0.586,   0.562,   0.537,   0.512,
                            0.488,   0.463,   0.438,   0.414,   0.390,   0.366,   0.342,   0.319,
                            0.296,   0.274,   0.252,   0.231,   0.210,   0.190,   0.171,   0.153,
                            0.136,   0.119,   0.104,   0.089,   0.075,   0.063,   0.051,   0.041,
                            0.032,   0.024,   0.017,   0.011,   0.006,   0.003,   0.001,   0.000,
                            0.000,   0.002,   0.005,   0.009,   0.014,   0.020,   0.028,   0.036,
                            0.046,   0.057,   0.069,   0.082,   0.096,   0.111,   0.127,   0.144,
                            0.162,   0.181,   0.200,   0.220,   0.241,   0.263,   0.285,   0.307,
                            0.330,   0.354,   0.378,   0.402,   0.426,   0.451,   0.475,   0.500};

class TESTBENCH : public sc_module{
public:
    SC_HAS_PROCESS(TESTBENCH);
    TESTBENCH(sc_module_name name) { 
        SC_THREAD(source);
        dont_initialize();
        sensitive << clk.pos();
        SC_THREAD(sink);
        dont_initialize();
        sensitive << clk.pos();
    }
    rdyvld_port_out<sc_ufixed_fast<53,10>> a,b;
    rdyvld_port_in<sc_ufixed_fast<53,10>> r;
    sc_in_clk clk;
private:
    void source();
    void sink();
    void monitor();
    sc_ufixed_fast<53,10> _a, _b, _r;
    //Store the previous inputs to FIFOs
    sc_int<WIDTH> t_a;
    bool t_a_rdy;
    bool t_a_vld;
    sc_int<WIDTH> t_b;
    bool t_b_rdy;
    bool t_b_vld;
    sc_int<WIDTH+1> t_sum;
    bool t_sum_rdy;
    bool t_sum_vld;
};

void TESTBENCH::source(){
    for (int i = 0; i < 128; i++){
        a.write(x_input_signal[i]);
        wait();
    }
}

void TESTBENCH::sink(){
    for (int i = 0; i < 128; i++){
        _r = r.read();
        cout << "y_downsample_by2 = " << _r << endl;
        wait();
    }
}

void TESTBENCH::monitor() {
    cout << setw(8) << "time" << setw(8) << "a" << setw(8) << "a rdy" << setw(8) << "a vld" << setw(8) << "b" 
         << setw(8) << "b rdy" << setw(8) << "b vld" 
         << setw(8) << "sum" << setw(8) << "sum rdy" 
         << setw(8) << "sum vld" << endl;
    while (true) {
      // read status, still need sink
      t_a=a.msg.read();
      t_a_rdy=a.rdy.read();
      t_a_vld=a.vld.read();
      t_b=b.msg.read();
      t_b_rdy=b.rdy.read();
      t_b_vld=b.vld.read();
      t_sum=r.msg.read();
      t_sum_rdy=r.rdy.read();
      t_sum_vld=r.vld.read();
      cout << setw(8) << sc_time_stamp();
      cout << setw(8) << t_a.to_string(SC_BIN);
      cout << setw(8) << t_a_rdy;
      cout << setw(8) << t_a_vld;
      cout << setw(8) << t_b.to_string(SC_BIN);
      cout << setw(8) << t_b_rdy;
      cout << setw(8) << t_b_vld;
      cout << setw(8) << t_sum.to_string(SC_BIN);
      cout << setw(8) << t_sum_rdy;
      cout << setw(8) << t_sum_vld << endl;
      cout << "===============================================================================================" << endl;
      wait();
    }
  }

#endif