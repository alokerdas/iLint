module test (a, b, y);
 input [7:0] a, b;
 output [7:0] y;
 reg [7:0] y;
 integer Nnnnnnnnnnnn; //warning
 always @(a)
   begin
   y=0;
   for (Nnnnnnnnnnnn=0; Nnnnnnnnnnnn<=7; Nnnnnnnnnnnn=Nnnnnnnnnnnn+1)
     y[Nnnnnnnnnnnn] = a[Nnnnnnnnnnnn] & b[Nnnnnnnnnnnn];
   end
endmodule

