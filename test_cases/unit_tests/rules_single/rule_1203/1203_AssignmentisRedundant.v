module block1( clk, a, b);
input clk, b;
output a;
reg a;
always @(posedge clk) begin
  a <= b;
  a <= ~b;
end
endmodule

