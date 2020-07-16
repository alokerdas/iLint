module test (c, a, b, en);
 input a, b, en;
 output c;
 reg c;
 always @(a or b)
   if (~en)
     c = a; //blocking assignment on "c"
   else
     c <= #2 b; //non-blocking assignment on "c"
endmodule

