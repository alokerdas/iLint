module test (q, clk, reset, d);
 output q;
 input clk, reset, d;
 reg q;
 wire clk, reset, d;
 always @(posedge clk)
   q <= d;
 always @(reset)
   if (~reset)
     assign q = 0; //procedural continuous assignment
               //non-synthesizable, warning
   else
     deassign q;
endmodule

