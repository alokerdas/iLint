module test;
reg a1, a2, a, b, c;
reg clk, sel, s;
reg q, q1, q2, d, d1;
always @* begin //always_comb preferred;
a = b - c;
a = b * c;
a = b / c;
a = b % c;
end
always_ff @( posedge clk ) begin //no warning, since it is inferred as a
q <= d;
end
always_latch @* //no warning, since it is inferred as a latch;
if ( sel )
q1 <= d1;
always_ff @* //always_comb preferred;
case (s)
1: q2 = a1;
0: q2 = a2;
endcase
endmodule
