module test;
 reg a, b, c, d, e;
 initial
 begin
   $monitor($time,,,"a=%d,b=%d,c=%d,d=%d,e=%d",a,b,c,d,e);
   assign d = a & b & c;
   a = 1;
   b = 0;
   c = 1;
   force d = (a | b | c);
   force e = (a | b | c);
   release d; //"release" non-synthesizable, warning
   release e; //"release" non-synthesizable, warning
   #10 $finish;
 end
endmodule

