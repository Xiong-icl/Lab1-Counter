## ChatGPT's code
```
module counter #(
  parameter WIDTH = 8
)(
  // interface signals
  input  logic             clk,      // clock
  input  logic             rst,      // reset
  input  logic             ld,       // load counter from data
  input  logic [WIDTH-1:0] v,        // value to preload
  output logic [WIDTH-1:0] count     // count output
);

  always_ff @ (posedge clk) begin
    if (rst) 
      count <= {WIDTH{1'b0}};           // Reset count to zero
    else if (ld)
      count <= v;                       // Preload count when ld is 1
    else
      count <= count + 1;               // Increment count when ld is 0
  end

endmodule
```
```
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {

    int i;
    Verilated::commandArgs(argc, argv);
    Vcounter* top = new Vcounter;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    if (vbdOpen() != 1) return -1;  // Open Vbuddy interface
    vbdHeader("Lab: Counter with Preload");

    top->clk = 1;
    top->rst = 1;
    top->ld = 0;           // Initially, no load
    top->v = 0;            // Load value initially 0

    // Simulation loop
    for (i = 0; i < 600; i++) {

        // Toggle clock and evaluate the model
        for (int clk = 0; clk < 2; clk++) {
            tfp->dump(2 * i + clk);    // Dump waveform to VCD
            top->clk = !top->clk;      // Toggle clock signal
            top->eval();               // Evaluate model
        }

        // Check Vbuddy flag (if button pressed)
        if (vbdFlag()) {
            top->ld = 1;               // Set load signal
            top->v = vbdValue();       // Load Vbuddy's parameter value
        } else {
            top->ld = 0;               // Clear load signal
        }

        // Plot the current count
        vbdPlot(int(top->count), 0, 255);

        // Set the reset signal
        top->rst = (i < 2) | (i == 15);

        // Handle simulation finish
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();             // Close Vbuddy
    tfp->close();           // Close VCD
    exit(0);
}
```
