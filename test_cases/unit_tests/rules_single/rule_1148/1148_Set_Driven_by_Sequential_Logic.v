module test (q, clk, rst, d, e);
output q;
reg c;
input rst, clk, d, e;
reg q;
always @(posedge clk or posedge rst)
if (rst)
c = 0;
else
c = d; //warning on "c[3]", set
//signal is an output of
//sequential device
always @(posedge clk or posedge c)
if (c)
q = 1;
else
q = e;
endmodule

