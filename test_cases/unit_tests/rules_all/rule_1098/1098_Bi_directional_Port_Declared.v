module test (a, b, c, d, f, f1, f2);
 input a, b, c, d;
 output f;
 inout f1, f2; //warning on "f1", "f2"
 and AA(f,f1,f2);
endmodule

