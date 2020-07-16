module test(in1,a);
input [3:6] in1;
input a;
endmodule
module top ();
wire [6:4] B;
test i_test(B, B[4]);
endmodule
