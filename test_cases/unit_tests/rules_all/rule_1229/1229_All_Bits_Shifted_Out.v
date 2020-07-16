module test (A, clk, num, b);
 input clk;
 input [7:0] A;
 input [7:0] num;
 output [7:0] b;
 reg [7:0] b;
 always @(posedge clk)
   b =  A << num;
endmodule

