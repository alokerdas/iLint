module test(c,a,b);
reg [8:0] mem[0:4095]; //warning here, memory length bits too large
input [11:0] a;
input b;
output [7:0] c;
reg [7:0] c;
always @( a )
  if ( b )
    c = mem[a];
endmodule

