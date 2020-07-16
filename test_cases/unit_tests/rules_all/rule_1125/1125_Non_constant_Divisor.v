module test (a, b, y);
 input [2:0] a, b;
 output [3:0] y;
 reg [3:0] y;
 always @(a or b)
   y=a/b;
endmodule

