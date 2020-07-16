module test (clk,a, b, y);
 input clk;
 input [1:0] a, b;
 output [3:0] y;
 reg [3:0] y;
 integer i;
 always @(clk)
   begin
   i = 0;
   while (i <= 3)//"while", warning
     begin
     y[i] = a[i];
     i = i + 1;
     end
   end
endmodule

