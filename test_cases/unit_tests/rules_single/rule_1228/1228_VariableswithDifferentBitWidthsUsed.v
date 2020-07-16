module test (a, b, c, d);
 input b;
 input [2:0] c;
 input [3:0] d;
 output [3:0] a;
 assign a = b ? c[2:1] : d[3:0];
endmodule

