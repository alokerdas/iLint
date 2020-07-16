module top;
 reg a, b, cin;
 wire sum;
 test u_test_0(sum, cin, a, b); //non-synthesizable, warning
endmodule
primitive test(sum, cin, a, b);
 output sum;
 input cin,a,b;
 table
 0 0 0 : 0;
 0 0 1 : 1;
 0 1 0 : 1;
 0 1 1 : 0;
 1 0 0 : 1;
 1 0 1 : 0;
 1 1 0 : 0;
 1 1 1 : 1;
 endtable
endprimitive

