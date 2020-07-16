module test (q, clk, en, reset, d);
output q;
input clk, en, reset, d;
reg q;
wire clk, en, reset, d;
wire clk_en;
and U_and_1(clk_en, clk, en);//warning
always @( posedge clk_en)//or negedge reset )
if ( ~reset )
q <= 1'b0;
else
q <= d;
endmodule

