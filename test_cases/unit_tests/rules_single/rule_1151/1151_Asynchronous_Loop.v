module test (count, clk);
 parameter number = 10;
 output [3:0] count;
 input clk;
 reg [3:0] count;
 reg i_rst;
 always @(posedge clk or posedge i_rst) //warning on
                     //"count->i_rst->count"
   if (~i_rst )
     count <= 0;
   else
     count <= count + 1;
 always @( count )
   if ( count == number )
     i_rst = 1;
   else
     i_rst = 0;
endmodule

