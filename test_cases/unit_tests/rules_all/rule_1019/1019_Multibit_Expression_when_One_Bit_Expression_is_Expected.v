module test (a, c);
 input [1:0] a;
 output c;
 reg c;
 always @(a)
   begin
   if (a) //warning here, one bit expression expected,
          //"a != 2'b00" is desired
     c = 1;
   else
     c = 0;
   end
endmodule

