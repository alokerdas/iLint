module test (counter, clock, reset);
 input clock, reset;
 output [3:0] counter;
 reg [3:0] counter;
 wire [3:0] tmp;
 pure_dff pure_dff1( tmp, clock, tmp+4'b1 );
 always @( reset or tmp )
   if ( ~reset )
     counter <= 0;
   else
     counter <= tmp;
endmodule
module pure_dff (q, clock, d);
 output [3:0] q;
 input clock;
 input [3:0] d;
 reg [3:0] q;
 always @(posedge clock)
   q = d; //when simulation begins, the value of "q" cannot
         //be easily determined, warning
endmodule

