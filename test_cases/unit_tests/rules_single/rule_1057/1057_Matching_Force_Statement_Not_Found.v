module top;
 reg a, b, c, d;
 wire e;
 initial
 begin
   assign d = a & b & c;
   a = 1;
   b = 0;
   c = 1;
   #10;
   release d; //"d" is released but never been forced, warning
   release e; //"e" is released but never been forced, warning
   #10 $finish;
 end
 test u_test_0(e, a, b, c);
endmodule
module test (e,a,b,c);
 output e;
 input a,b,c;
 wire e;
 and and1(e,a,b,c);
endmodule

