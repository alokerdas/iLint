module test (count, c, clk, a, b);
input clk, a, b;
output [8:0] count;
output c;
reg c;
reg [8:0] count;
always @( posedge clk )
c = a; //"c" is output of registe, warning here
always @(posedge clk or posedge c)
if (c) //"c" is used as reset
count = 0;
else
count = b;
endmodule
