module test (a, clk, num, b);
 input clk;
 input [7:0] a;
 input [1:0] num;
 output b;
 reg b;
 always @(posedge clk)
   b = a[num]; //warning
endmodule

