module test (a,c);
  input [7:0] a;
  output [7:0]c;
  reg [7:0]c;
  integer i;
  always@(a) begin
    for (i=0;i<7;i=i-1) //warning
      c[i]=1;
  end
endmodule

