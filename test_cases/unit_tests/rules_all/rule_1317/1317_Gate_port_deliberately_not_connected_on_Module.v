module test();
wire ta, tb;
uu u_uu_1( .a(ta), .b() ); // warning here
endmodule

module uu( a, b );
output a;
input b;
not N(a,b);
endmodule
