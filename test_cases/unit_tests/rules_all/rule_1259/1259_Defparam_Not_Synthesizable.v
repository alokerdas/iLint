module top (a,b,clk);
input [6:0] a;
input clk;
output [6:0] b;
sub m1 (.clk(clk), .in(a), .out(b));
endmodule
module sub (in,clk,out);
parameter size = 8,delay = 2;
input [size-1:0] in;
input clk;
output [size-1:0] out;
reg [size-1:0] out;
always @(posedge clk)
#delay assign out = in;
endmodule
module annotate;
defparam //non-synthesizable, warning
top.m1.size = 5 ,
top.m1.delay = 10;
endmodule

