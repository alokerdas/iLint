module dffn( clk, d, q );
 input clk, d;
 output q;
 reg q;
 always @( negedge clk ) //warning here
   q <= d;
endmodule
module test( clock, d, q );
 input clock;
 input [3:0] d;
 output [3:0] q;
 wire clkn;
 assign clkn = ~clock;
 dffn i_dffn_1( clkn, d[0], q[0] );
 dffn i_dffn_2( clock, d[1], q[1] );
endmodule

