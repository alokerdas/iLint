module test (y, count, clock, reset);
 input clock, reset;
 output [8:0] count;
 reg [8:0] count;
 output y;
 initial
   count <= 0;
 always @(posedge clock or negedge reset)
   begin
   if (reset)
     count = 0;
   else
     count = count + 1;
   end
 assign y = reset;
endmodule

