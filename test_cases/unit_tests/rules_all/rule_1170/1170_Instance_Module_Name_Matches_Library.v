module testini;
 wire a,b,c;
 JANIV JANIV1(a,b,c);
endmodule
 module JANIV(a,b,c);
 input a,b;
 output c;
 and and1 (c,a,b);
endmodule

