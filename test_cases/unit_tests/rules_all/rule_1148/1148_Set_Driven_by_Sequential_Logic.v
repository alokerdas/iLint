module test (q, clk, c, d);
output q;
output [3:0] c;
input clk, d;
reg q;
reg [3:0] c;
always @(posedge clk)
c <= c + 1; //warning on "c[3]", set
//signal is an output of
//sequential device
always @(posedge clk or posedge c[3])
if (~c[3])
q <= 1;
else
q <= d;
endmodule

