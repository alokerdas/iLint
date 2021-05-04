module test (clk, en, d, q2, q3, q1);
input clk, en, d;
output q2, q3, q1;
wire clk1, clk, d, en, clk2;
reg q1, q2, q3;
always @(posedge clk)
q1 = d;
always @(posedge clk1)
q2 = d;
assign clk1 = en ? clk : 'bZ;
always @(posedge clk2)
q3 = d;
assign clk2 = en ? clk : 'bZ;
endmodule
