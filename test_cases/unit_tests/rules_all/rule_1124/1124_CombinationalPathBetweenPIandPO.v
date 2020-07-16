module test (a, b, c, k);
 input a;
 input b;
 output c;
 input [2:0] k;
 wire c;
 assign c = a & b & k[0];
endmodule

