module test (d, e, a, b);
 input a, b;
 output d,e;
 reg d,e;
 always @( a )
   begin
    d = 0; //warning
    e = a & ~a; //warning
   end
endmodule

