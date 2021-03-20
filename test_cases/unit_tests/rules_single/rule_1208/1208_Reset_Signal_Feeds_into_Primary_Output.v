module test (y, count, clock, reset);
 input clock, reset;
 output [8:0] count;
 reg [8:0] count;
 output y;
 always @(posedge clock or posedge reset)
   begin
   if (reset)
     count = 0;
   else
     count = count + 1;
   end
 assign y = reset;
endmodule

