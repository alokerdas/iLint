module test (d, e, a, b);
 input a, b;
 output d, e;
 reg d, e;
 wire a, b;
 always @( a )
   begin
    d = 1; //warning
    e = a | ~a; //warning
   end
endmodule

