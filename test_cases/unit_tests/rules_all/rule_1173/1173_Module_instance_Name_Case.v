module top;
 wire a, b;
 EA U_EA (a, b); //warning on "U_EA"
endmodule
module EA (a, b);
 input a;
 output b;
endmodule

