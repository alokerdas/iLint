module test (sel,b);
input [1:0] sel;
output [1:0] b;
reg
[1:0] b;
always @(sel)
case(sel) //synopsys full_case , warning here
2'b00:
b = 2'b01;
2'b01:
b = 2'b10;
default: b = 2'b11;
endcase
endmodule

