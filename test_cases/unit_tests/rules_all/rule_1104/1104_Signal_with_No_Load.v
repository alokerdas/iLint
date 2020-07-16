module test (c, a, b);
 input [1:0] a, b;
 output [1:0] c;
 reg [1:0] c;
 wire e; //warning
 wire d; //warning
 assign e = 1;
 assign d = 0;
 always @(a or b)
   c = a + b;
endmodule

