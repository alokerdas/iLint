module test;
 reg a,b,c,d,e;
 initial
 begin
   $monitor($time,,,"a=%d,b=%d,c=%d,d=%d,e=%d",a,b,c,d,e);
   assign d = a & b & c;
   a = 1;
   b = 0;
   c = 1;
   force d = (a | b | c); //"force" non-synthesizable, warning
   force e = (a | b | c); //"force" non-synthesizable, warning
   release d;
   release e;
   #10 $finish;
 end
endmodule

