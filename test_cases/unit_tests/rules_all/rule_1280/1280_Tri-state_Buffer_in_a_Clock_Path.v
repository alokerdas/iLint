module test (clk, en, d, q);
input clk, en, d;
output q;
wire clk1, clk, d, en, clk2;
reg q;
always @(posedge clk)
q = d;
always @(posedge clk1)
q = d;
assign clk1 = en ? clk : 'bZ;
always @(posedge clk2)
q = d;
assign clk2 = en ? clk : 'bZ;
endmodule
