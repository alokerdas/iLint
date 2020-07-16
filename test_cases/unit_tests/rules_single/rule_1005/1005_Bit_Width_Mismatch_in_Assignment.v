module test(a, b);
output [3:0] a;
input [2:0] b;
assign a = b; //warning on "a" and "b"
endmodule

