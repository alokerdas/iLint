module test (out0,in1,in2,sel);
 input sel;
 input [1:0] in1,in2;
 output [1:0] out0;
 reg [1:0] out0;
 always @( in1 or in2 or sel)
   if ( sel == 1'b1 )
     out0 = (in1 > -2'd1); //warning here
   else
     out0 = in2;
endmodule

