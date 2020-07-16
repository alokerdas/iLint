module test (clk,rst,count);
 input clk,rst;
 output [8:0] count;
 reg [8:0] count;
 reg [8:0] nextCount;
 always @(posedge clk or negedge rst)
   if (~rst) //good style for using '_n' for active low reset
     count <= 0;
   else
     count <= nextCount;
endmodule

