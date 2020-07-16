module test (count, clk, rst_p);
 input clk, rst_p;
 output [7:0] count;
 reg [7:0] count;
 always @(posedge clk or clk //warning on "clk"
          or posedge rst_p)
   if (rst_p)
     count = 0;
   else
     count = 1;
endmodule

