module top;
  reg a, b;
  wire c, d, e;
  initial
    begin
    a = 0;
    b = 1;
    #100 $finish;
    end
  always
    #5 a = ~a;
  and U_and(c, a, b); //gate "and" in non-leaf module will cause
                       //systhesizer optimization problem, warning
  test U_test(a, b, d, e);
endmodule
module test (a,b,c,d);
  input a,b;
  output c,d;
  reg c;
  wire d;
  always @(a or b)
    c = ~b;
  and and1(d,a,b);
endmodule

