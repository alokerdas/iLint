module test( m, n, o );
 input m, n;
 output o;
 assign o = m & n;
endmodule
module test1( x, y, z );
 input x, y;
 output z;
 assign z = x | y;
endmodule
module test2( a, b, c );
 input a, b;
 output c;
 test u1( c, b, c ); //here signal connected to output "c" goes into
               //input "a" of test1 again.
 //or something like following
 test1 u2( a&c, b, c);
endmodule

