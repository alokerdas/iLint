module block( clk, a, b);
input clk, b;
output a;
reg a;
always @(posedge clk) begin
  a <= 1;
  if (b)
    a <= 0;
end
endmodule

