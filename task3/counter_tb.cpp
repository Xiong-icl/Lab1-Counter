#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){

    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    Vcounter* top = new Vcounter;
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    if(vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1:Counter");

    top->clk = 1;
    top->rst = 1;
    top->count;
    top->v; //value to preload
    top->ld;  //load counter from data

    for(i = 0; i < 600; i++){

        vbdSetMode(1);
        if(vbdFlag()){

            top->ld = 1;
            top->v = top->count;
        }
        else{

            top->ld = 0;
        }

        for(clk = 0; clk < 2; clk++){

            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }

        vbdPlot(int(top->count), 0, 255);

        vbdCycle(i+1);

        top->rst = (i < 2) | (i == 15);
        // top->en = vbdFlag();

        if(Verilated::gotFinish()){
            exit(0);
        }
        
    }

    vbdClose();
    tfp->close();
    exit(0);
}