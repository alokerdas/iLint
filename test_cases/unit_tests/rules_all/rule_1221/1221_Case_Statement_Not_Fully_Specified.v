module test (out0, in1, in2, in3, sel);
 input [1:0] in1, in2, in3, sel;
 output [1:0] out0;
 reg [1:0] out0;
 always @(in1 or in2 or sel)
   case (sel) //warning
    2'b00: out0 = in1;
    2'b01: out0 = in2;
    2'b10: out0 = in3;
   endcase
endmodule

