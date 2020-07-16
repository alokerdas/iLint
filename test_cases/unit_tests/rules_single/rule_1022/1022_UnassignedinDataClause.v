module test (counter, clock, reset);
 input clock, reset;
 output [3:0] counter;
 reg [3:0] counter;
 wire [3:0] tmp;
 always @(posedge clock or negedge reset)
   if ( ~reset )
     counter = 0; //warning here
endmodule

