/*module test (c, a, b);
 input [1:0] a, b;
 output [1:0] c;
 reg [1:0] c;
 always @(a or b)
   if (a)
     c = b;
   else
     c = 1'bz; //tri-state "c" inferred
endmodule
*/
module test (c, a, b);
 input [1:0] a, b;
 output [1:0] c;
 reg [1:0] c;
 always@(a or b)
   if (a)
     c = b; //latch "c" inferred
endmodule

