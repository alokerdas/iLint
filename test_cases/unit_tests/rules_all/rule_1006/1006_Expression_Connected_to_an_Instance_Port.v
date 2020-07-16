module test;
 wire a,b1,b2;
 wire c;
 and and1(c, a, b1+b2); //warning on "b1+b2" as port instance;
endmodule

