module test (clk, a, tri_, R);
input clk, a, tri_;
output R;
wire clk, a;
reg R, R1;
wire c, d, e;
always @(posedge clk)
R1 = a;
assign c = R1;
assign d = tri_ ? c : 1'bz;
always @(posedge clk)
R = d;
endmodule

