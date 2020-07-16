module test (counter, data, clock, reset);
 input clock, reset, data;
 output counter;
 reg counter;
 always @(posedge clock or posedge reset)
   if (reset)
     counter = 0;
   else
     counter = data;
endmodule
