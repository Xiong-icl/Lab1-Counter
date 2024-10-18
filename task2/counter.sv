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
