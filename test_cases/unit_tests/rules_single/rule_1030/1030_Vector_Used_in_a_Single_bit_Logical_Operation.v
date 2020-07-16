module test (.pc(c), .pa(a), .pb(b));
 input [2:0] a, b;
 output [2:0] c;
 reg [2:0] c;
 always @(a or b)
   begin
   if (a && b) //warning on "a" and "b"
     c = 1;
   if (!(a || b)) //warning on "a" and "b"
     c = 0;
   end
endmodule

