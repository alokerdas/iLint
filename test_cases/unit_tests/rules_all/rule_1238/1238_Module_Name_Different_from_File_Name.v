module testini;
wire a,b,c;
test u_test_0 (a,b,c);
endmodule
module test(a,b,c);
input a,b;
output c;
and and1 (c,a,b);
endmodule

