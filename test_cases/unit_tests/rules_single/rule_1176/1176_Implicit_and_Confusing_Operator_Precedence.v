module test (f1, f2, f3, a, b, c, d, e, f, g, h, i, j);
 input a, b, c, d, e, f, g, h, i, j;
 output f1, f2, f3;
 reg f1, f2, f3;
 always @(a or b or c or d or e or f or g or h or i or j)
   begin
   if ( a != b & c)
     f1 = a << 2 | b;
   else
     f1 = b << 2 + c;
   f3 = g + h ? i : j;
   end
endmodule

