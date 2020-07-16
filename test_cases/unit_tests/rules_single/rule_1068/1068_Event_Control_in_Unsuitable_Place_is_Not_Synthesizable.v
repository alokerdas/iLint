module test (clk, data, y);
 input clk, data;
 output y;
 reg y;
 always @(posedge clk)
   @( data ) y = data;//"@(data)" non-synthesizable, warning
endmodule

