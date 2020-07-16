module top (c,a,b);
 input a, b;
 output c;
 block test_0(c, a, b); //warning on test_0, suggest to use u_test_0
endmodule
module block (c, a, b);
 input a, b;
 output c;
endmodule

