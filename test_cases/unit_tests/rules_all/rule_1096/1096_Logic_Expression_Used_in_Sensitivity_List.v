module test (count, clk1, clk2, rst_p);
 input clk1, clk2, rst_p;
 output [7:0] count;
 reg [7:0] count;
 always @(posedge(clk1|clk2) or //warning
         posedge (rst_p))
   if (rst_p)
     count = 0;
   else
     count = count + 1;
endmodule

