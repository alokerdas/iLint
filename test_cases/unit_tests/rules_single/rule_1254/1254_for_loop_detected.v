module test (q, clk, s );
parameter WIDTH = 8;
output [WIDTH:0] q;
input [2:0] s;
input clk;
reg [WIDTH:0] q;
integer I;
always @(posedge clk)
for (I=1; I<s; I = I+1)
q[I] <= q[I-1]; //loop count is not constant, warning
endmodule

