module test (q, clk, en, reset, d);
output q;
input clk, en, reset, d;
reg q;
wire clk, en, reset, d;
wire rst_en;
and U_and_1(rst_en, reset, en);//warning on "rst_en", is gated
always @( posedge clk or negedge rst_en )
// always @(clk | rst_en )
if (rst_en )
q <= 1'b0;
else
q <= d;
endmodule

