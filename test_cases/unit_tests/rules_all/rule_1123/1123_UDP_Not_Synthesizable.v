module test_M(a,b,c,d);
 input a,b,c;
 output d;
test test1 (d,a,b,c);
endmodule
primitive test(sum, cin, a, b);//non-synthesizable, warning
  output sum;
  input cin, a, b;
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

