module test (q, clk, rst, d);
input clk, rst;
input d;
output q;
reg q;

always @(posedge clk or negedge rst)
if (~rst)
q <= 1;
else
q <= d;

endmodule
