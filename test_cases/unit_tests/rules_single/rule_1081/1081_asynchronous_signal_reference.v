module test (a, b, clk, reset, chk);
  output a;
  input b, clk, reset, chk;
  reg a;
  always @(posedge clk or posedge reset or posedge chk)
  if (reset)
    a <= 0;
  else if (chk)
    a <= b; //warning here
  else
    a <= a;
endmodule
