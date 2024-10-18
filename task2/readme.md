##Task 2
By running the new commands to get the VBuddy name, we can modify the testbench to visualise the counter we created on the VBuddy directly.

```
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

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

    if(vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1:Counter");

    top->clk = 1;
    top->rst = 1;
    top->en = 0;
    top->count;

    for(i = 0; i < 600; i++){

        for(clk = 0; clk < 2; clk++){

            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }

        vbdPlot(int(top->count), 0, 255);

        vbdCycle(i+1);

        top->rst = (i < 2) | (i == 15);
        top->en = vbdFlag();

        if(Verilated::gotFinish()){
            exit(0);
        }
        
    }

    vbdClose();
    tfp->close();
    exit(0);
}
```
```
module counter #(
    parameter WIDTH = 8
)(
    input logic clk,
    input logic rst,
    input logic en,
    output logic [WIDTH-1:0] count
);

always_ff @ (posedge clk)
    if(rst) count <= {WIDTH{1'b0}};
    else if (en) count <= count + 1;
    else count <= count - 1;

endmodule

```
The vbdPlot is used to plot the count whereas the vbdHex allows for the count to be visualised as a 4 digit count.
The new SystemVerilog code allows the count to decrease when enable is 0.
