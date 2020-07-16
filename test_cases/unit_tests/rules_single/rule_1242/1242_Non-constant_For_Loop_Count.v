/*module test (a,c);
  input [7:0] a;
  output [7:0]c;
  reg [7:0]c;
  integer i;
  always@(a) begin
    for (i=0;i<7;i=i-1) //warning
      c[i]=1;
  end
endmodule*/

module test (q, clk, s );
  parameter WIDTH = 8;
  output [WIDTH:0] q;
  input [2:0] s;
  input clk;
  reg [WIDTH:0] q;
  integer I;
  always @(posedge clk)
    for (I=1; I<s; I = I+1)
      q[I] <= q[I-1]; //loop count is not constant, warning
endmodule


