module test (a, inc_dec, sum);
 input a, inc_dec;
 output [7:0] sum;
 reg [7:0] sum;
 always @(a or inc_dec)
   begin//: COMBINATIONAL_PROC
	   sum = 8'b0;
   if ( inc_dec == 0)
     sum = a ;
   else
     sum = a ;
   end //good coding style using separate line for each HDL statement
endmodule

