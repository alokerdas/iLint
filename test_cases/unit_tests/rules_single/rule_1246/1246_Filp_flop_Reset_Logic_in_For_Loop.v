module smp (out1, in1, clk, reset);
 output [3:0] out1;
 input [3:0] in1;
 input clk, reset;
 reg [3:0] out1;
 integer i;
  always @ (posedge clk or negedge reset)
   for (i = 0; i < 4; i = i + 1)
    if ( !reset )
      out1[i] = 1'b0;
    else
      out1[i] = in1[i];
endmodule

