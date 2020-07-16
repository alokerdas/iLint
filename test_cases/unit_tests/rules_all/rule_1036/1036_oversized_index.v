module test(a, clk, num, b);
 input clk;
 input [3:0] a;
 input [4:0] num;
 output b;
 reg b;
 always @(posedge clk)
   b = a[num]; //warning on "a[num]"
endmodule

