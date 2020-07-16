module test (c, a);
 input a;
 output c;
 reg c;
 always @(a)
   c = (&a); //warning on "&a",
          //reduction operation on one bit signal
endmodule

