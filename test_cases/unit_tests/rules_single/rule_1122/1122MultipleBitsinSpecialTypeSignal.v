module test (q, clk, rst, in);
 input in, clk;
 input [1:0] rst;
 output q;
 reg q;
 always @(posedge clk)
   if (rst) //reset signal "rst" is multiple bit signal
     q <= 0;
   else
     q <= in;
endmodule

