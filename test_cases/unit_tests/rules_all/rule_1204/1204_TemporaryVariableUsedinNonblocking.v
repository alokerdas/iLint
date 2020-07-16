module test (clk, y, a, b, c, d);
 output y;
 input a, b, c, d;
 input clk;
 reg y, tmp1, tmp2;
 always @(posedge clk)
 begin
   tmp1 <= a & b;
   tmp2 <= c & d;
   y <= tmp1 | tmp2;
end endmodule

