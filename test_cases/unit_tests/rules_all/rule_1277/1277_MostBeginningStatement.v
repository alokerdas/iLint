module test(d1, clk, R, d, q, d2);
 input d1, clk, R, d;
 output q, d2;
 reg q, d2;
 always @( posedge clk or posedge R) begin
   d2 = d1;
   if (R) // not the first statement
     q = 0;
   else
     q = d;
 end
endmodule

