module test (a, b, y);
 input [7:0] a, b;
 output [7:0] y;
 reg [7:0] y;
 integer N; //warning on 'N' if CASE_LOWER
 always @(a)
   begin
   y=0;
   for (N=0; N<=7; N=N+1)
     y[N] = a[N] & b[N];
   end
endmodule

