module test (clk, d, q);
 input clk, d;
 output q;
 reg q;
 always@(posedge clk)
   q = d; // register inferred not in library
endmodule

