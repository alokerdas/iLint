module test (out0,in1,in2,sel);
 input sel;
 input in1,in2;
 output out0;
 reg out0;
 always @( in1 or in2 or sel)
   out0 = sel ? in1 : in2; //warning here
endmodule

