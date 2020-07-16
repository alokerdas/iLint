module test (A, b, C);
 input A, b; //warning on 'A' if CASE_LOWER
 output C;
 wire C; //warning on 'C' if CASE_LOWER
 and and1(C,A,b);
endmodule

