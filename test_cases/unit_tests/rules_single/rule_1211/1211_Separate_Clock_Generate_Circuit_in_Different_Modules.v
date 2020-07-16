module test;
 wire clk, c1, c2;
 reg d, q;
 assign clk = c1 & c2;
 always @(posedge clk)
  q = d;
endmodule

