module test (q1, q2, clk_1, enable, d1, d2);
output q1, q2;
input clk_1, enable, d1, d2;
reg q1, q2;
wire clk_1, enable, d1, d2;
always @( posedge clk_1 or posedge enable )
if ( enable ) //"enable" is used as reset
q1 <= 1'b0;
else
q1 <= d1;
always @( posedge clk_1 or posedge enable )
if ( enable ) //warning here, "enable" is also used as
//set signal
q2 <= 1'b1;
else
q2 <= d2;
endmodule

