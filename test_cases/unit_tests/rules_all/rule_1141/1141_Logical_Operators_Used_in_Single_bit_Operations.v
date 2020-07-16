module smp(out, in1, in2, in3);
 output out;
 input in1, in2, in3;
  assign out = !(in1 & in2 | in3);
endmodule

