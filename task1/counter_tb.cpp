#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env){

    int i;
    int a = 0;
    int clk;

    Verilated::commandArgs(argc, argv);
    Vcounter* top = new Vcounter;
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    top->clk = 1;
    top->rst = 1;
    top->en = 0;
    top->count;

    for(i = 0; i < 300; i++){
        
        if(top->count == 9){

            if(a < 3){

                top->en = 0;
                a++;
            }
            else{

                top->en = 1;
            }
        }

        for(clk = 0; clk < 2; clk++){

            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }

        top->rst = (i < 2) | (i == 15);
        top->en = (i > 4);

        if(Verilated::gotFinish()){
            exit(0);
        }
        
    }
    tfp->close();
    exit(0);
}