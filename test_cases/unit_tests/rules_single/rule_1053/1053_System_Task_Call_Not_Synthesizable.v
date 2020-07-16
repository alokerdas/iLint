module init;
    reg clk, rst, in;
    wire out;
    initial begin
     $monitor($time,,,"clk=%d,rst=%b,in=%b,out=%b",clk,rst,in,out);
     clk = 0;
     rst = 1;
     in = 0;
     #2 rst = 0;
     #10 rst = 1;
     #50 $finish;       //warning here
   end
   always
     #4 clk = !clk;
   always
    #10 in = !in;
  endmodule

