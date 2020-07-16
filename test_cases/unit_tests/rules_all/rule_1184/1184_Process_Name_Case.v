module test (a, inc_dec, sum);
 input a, inc_dec;
 output [7:0] sum;
 reg [7:0] sum;
 always@(a)
   begin: COMBINATIONAL_PROC
   if ( inc_dec == 0)
     sum = a ;
   else
     sum = a ;
   end
endmodule

