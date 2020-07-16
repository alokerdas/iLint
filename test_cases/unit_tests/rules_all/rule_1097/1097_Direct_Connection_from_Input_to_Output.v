module test (a, c,b,d);
 input a,b; //warning on 'a', is directly connected to output signal 'c'
 output c,d;
 reg c,d;
 always @(a)
 begin
   c = a;
 end
endmodule

