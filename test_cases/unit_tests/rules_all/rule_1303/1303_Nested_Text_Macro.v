`define macro1 1
`define macro2 `macro1
module smp(A,B,Y,Z);
input A,B;
output Y,Z;
assign Z = ( A == `macro1 ) ? 1'b1 : 1'b0;
assign Y = ( B == `macro2 ) ? 1'b0 : 1'b1;
endmodule

