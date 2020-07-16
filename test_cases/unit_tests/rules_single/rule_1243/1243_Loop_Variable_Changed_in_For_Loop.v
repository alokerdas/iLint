module test (a,b,y);
  parameter loopcount = 3;
  input [loopcount:0] a,b;
  output [loopcount:0] y;
  reg [loopcount:0] y;
  integer i;
  always @(a or b) begin
    for ( i=0; i<=loopcount; i=i+1) begin
      y[i] = a[i];
      i = i+1; //loop variable "i" changed in loop, warning
    end
  end
endmodule

