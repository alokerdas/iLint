module test (q1, q2, clk_1, clk_2, set, d1, d2);
output q1, q2;
input clk_1, clk_2, set, d1, d2;
reg q1, q2;
wire clk_1, clk_2, set, d1, d2;
always @( posedge clk_1 or negedge set )
if ( ~set )
q1 <= 1'b1;
else
q1 <= d1;
always @( posedge clk_2 or posedge clk_1 )
if ( clk_1 ) //warning
q2 <= 1'b1;
else
q2 <= d2;
endmodule
