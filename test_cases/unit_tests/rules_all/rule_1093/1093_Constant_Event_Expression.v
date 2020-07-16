module test (a, y);
 input [7:0] a;
 output [7:0] y;
 reg [7:0] y;
 parameter c = 1;
 always @(c) //warning
   y = a;
endmodule

