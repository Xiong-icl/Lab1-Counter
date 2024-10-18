Below is the testbench used for the counter modified to turn off EN for 3 cycles after counter reaches 0x9. EN is re-enabled after 3 cycles.

`#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"`

`int main(int argc, char **argv, char **env){`

    `int i;
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
    
    for(i = 0; i < 300; i++){`
        
        `//This function turns EN off for 3 cycles when the counter reaches 0x9, as shown by 'a' going from 0->3 in the loop
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
    exit(0);`
`}`
#### Below is the explanation of the code without the new function

![[Pasted image 20241017111726.png]]
`top->` accesses the SystemVerilog file to use the inputs in the testbench.

#### The SystemVerilog file is as shown:
`module counter #(`

    `parameter WIDTH = 8`

`)(`

`    input logic clk,
    input logic rst,
    input logic en,
    output logic [WIDTH-1:0] count
);`

`always_ff @ (posedge clk)`
`    if(rst) count <= {WIDTH{1'b0}};`
`    else count <= count + {{WIDTH-1{1'b0}}, en};
`
`endmodule`

A new line is required for the SystemVerilog file to read because the new line allows the translation of the file to be complete. If the new line was not there, the program will not read the `endmodule` and not terminate the module.

`always_ff` is a clockedge rising where if reset is detected, the count will return to `{WIDTH{1'b0}}` (or 0), else the count will continue to increase by `{WIDTH-1{1'b0}}` (or 1) whenever EN is on.

#### Synchronous and Asynchronous Resets
To implement the asynchronous reset, the reset will just have to depend on multiple factors, not just the clock cycle.
