module test_1301 (a, b, c);
  input a, b;
  output c;

  wire \cpu[0] ; //warning here

  and (\cpu[0] , a, b);
  not (c, \cpu[0] );
endmodule

