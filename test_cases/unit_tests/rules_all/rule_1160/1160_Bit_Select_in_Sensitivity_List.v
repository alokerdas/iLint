module test (d, clock, reset, q);
input d;
input [1:0] clock, reset;
output q;
reg q;
always @( posedge clock[0] or negedge reset[1] ) //warning here
if ( reset[1] )
q <= 1'b0;
else
q <= d;
endmodule
