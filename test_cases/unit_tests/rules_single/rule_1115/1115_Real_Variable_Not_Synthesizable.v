module test(a, b);
 input b;
 output a;
 real c; //warning
 assign a = c;
 function real testfun; //warning
  input p, q, cin;
  testfun = p + q + cin;
 endfunction
endmodule

