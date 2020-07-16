module dff (clk, i, o);
input clk, i;
output o;
reg o;
always @(posedge clk)
o <= i;
endmodule
module comb (i1, i2, o);
input i1, i2;
output o;
assign o = i1 & i2;
endmodule
module top (clk, i, o);
input clk, i;
output o;
wire a1, a2, b1, b2, c1, c2, d;
dff a_dff (.clk(clk), .i(i), .o(a1));
comb a_comb (.i1(a1), .i2(a2), .o(b1));
comb b_comb (.i1(b1), .i2(b2), .o(c1));
comb c_comb (.i1(c1), .i2(c2), .o(d));
dff c_dff (.clk(clk), .i(d), .o(o));
endmodule
