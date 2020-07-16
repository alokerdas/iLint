module test (out, clk, rst, in);
  input clk, rst, in;
  output out;
  reg out;
  always @(posedge clk)
    wait(rst) out = in; //"wait" non-synthesizable, warning
endmodule

