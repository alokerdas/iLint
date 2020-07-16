module test (q, clock, reset, d);
  output q;
  input clock, reset, d;
  reg q;
  wire clock, reset, d;
  always @(posedge clock or negedge reset)
    if (reset === 0) //case equal "===" non-synthesizable, warning
      q = 0;
    else
      q = 1;
endmodule

