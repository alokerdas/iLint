module test;
 reg a,b,c;
 always @(a) //warning
  c = a + b;
endmodule

