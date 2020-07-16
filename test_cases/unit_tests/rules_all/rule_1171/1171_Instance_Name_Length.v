module top;
 wire a, b;
 EA u_EA_012345678901234 (a, b); //warning on "u_EA_012345678901234"
endmodule
module EA (a, b);
 input a;
 output b;
endmodule

