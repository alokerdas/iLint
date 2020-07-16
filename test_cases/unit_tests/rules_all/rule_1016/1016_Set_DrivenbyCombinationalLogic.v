module test (q, clk, en, set, d);
 output q;
 input clk, en, set, d;
 reg q;
 wire clk, en, set, d;
 wire set_en;
 parameter P = 2;
 and U_and_1(set_en, set, en); //warning on "set_en", is gated
 always @( posedge clk or negedge set_en )
   if (~set_en )
     q <= 1'b1;
   else
     q <= d;
endmodule

