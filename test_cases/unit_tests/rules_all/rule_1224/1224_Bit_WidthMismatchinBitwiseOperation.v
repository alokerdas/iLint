module test (result, a, b);
 input [2:0] a;
 input [3:0] b;
 output [3:0] result;
 reg [3:0] result;
 always @(a or b)
   result = a & b; //warning on "a" and "b"
endmodule

