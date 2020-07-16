module test (q, clk, reset, d);
output q;
input clk, reset, d;
reg q;
wire clk, reset, d;
wire clk_i;
buf U_buf_1(clk_i, clk);//warning on "clk_i", clk_i is drived
//by a buffer
always @( posedge clk_i or negedge reset )
if (~reset )
q <= 1'b0;
else
q <= d;
endmodule

