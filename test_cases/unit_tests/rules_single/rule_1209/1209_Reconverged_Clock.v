module test(a, y);
 input a;
 output y;
 wire b,c,d,d1,f;
 reg y;
 always @(posedge a)
  y=d;
 and (b,a,f);
 and (c,a,f);
 and (d1,b,c); //warning here
endmodule

