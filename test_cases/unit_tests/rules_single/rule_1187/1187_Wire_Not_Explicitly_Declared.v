module test (a, b, c, d, f);
 input a, b, c, d;
 output f;
 wire f1, f2;
 assign d = f;
 and and1(f1, a, b); //warning on "f1", implicit wire
 or or1(f2, c, d); //warning on "f2", implicit wire
 or or2(f, f1, f2);
endmodule

