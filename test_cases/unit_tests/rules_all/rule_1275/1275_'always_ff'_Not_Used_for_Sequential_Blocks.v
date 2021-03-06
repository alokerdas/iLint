module test;
reg clk, sel;
reg a,b,c,d,d1;
reg q, q1;
always @* begin //no violation because it is inferred as combination logic
a = b & c;
end
always @( posedge clk ) begin //report warning
q1 <= d1;
end
always @* begin //no warning, since it is inferred as a latch;
if ( sel )
q <= d;
end
endmodule
