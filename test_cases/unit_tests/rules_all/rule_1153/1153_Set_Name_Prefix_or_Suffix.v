module test (q, clk, set, d);
output q;
input clk, set, d;
reg q;
always @( posedge clk or posedge set )
if (set )
q = 1'b1;
else
q = d;
endmodule

