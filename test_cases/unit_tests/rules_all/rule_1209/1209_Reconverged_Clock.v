module test;
 wire a,b,c,d,d1,f;
 reg y;
 always @(posedge a)
  y=d;
 always @(posedge b)
  y=d;
 always @(posedge c)
  y=d;
 and (b,a,f);
 and (c,a,f);
 and (d1,b,c); //warning here
 always @(posedge d1)
  y=d;
 always @(negedge d1)
  y=d;
endmodule

