module forkjoin1(clk, a, b);
  input clk;
  output a;
  output b;
  reg a, b;
  initial
  begin
     a = 0;
     b = 0;
  end
  always @(posedge clk)
  fork:A
     #2 a = 1;
     #1 b = a;
  join
endmodule

