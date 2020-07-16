module test (count, clk);
 parameter number = 10;
 output [3:0] count;
 input clk;
 reg [3:0] count;
 reg i_rst;
 always @(posedge clk)
   count <= count + 1; //warning here
endmodule

