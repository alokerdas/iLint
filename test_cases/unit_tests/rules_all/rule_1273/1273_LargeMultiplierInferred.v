module smp(IN1, IN2, OUT1, OUT2);
 input [16:0] IN1;
 input [15:0] IN2;
 output [16:0] OUT1;
 output [15:0] OUT2;
 reg [16:0] OUT1;
 reg [15:0] OUT2;
always @(IN1 or IN2)
   if(IN1 < IN2)
     OUT1 = IN1 * 2; //warning here if width argument is set to 16
   else
     OUT1 = IN1 / 2;
always @(IN1 or IN2)
   if(IN2 > IN1)
     OUT2 = IN2 * 4; //no warning here
   else
     OUT2 = IN2 / 4;
endmodule

