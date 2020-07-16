module test (clock, reset, a, b, c, d, in1, in2, out);
 input clock;
 input reset;
 input a, b, c, d, in1, in2;
 output out;
 reg out;
 always @(posedge clock) begin
   if ( ( a & b )== 0 ) // warning here
     out = 0;
   else if ( c & d ) // warning here
     out = 1;
   else
     out = ( in1 & in2 ); // warning here
 end
endmodule

