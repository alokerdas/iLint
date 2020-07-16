module test (q, clk, rst, in);
 input in, clk;
 input [1:0] rst;
 output q;
 reg q;
 always @(posedge clk)
   if (~rst[1]) //reset signal "rst[1]" is a bit select
     q <= 0;
   else
     q <= in;
endmodule

