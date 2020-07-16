module test (clk, data, y);
 input clk, data;
 output y;
 reg y;
 always @(posedge clk)
   #10 y = data;//"#10" ignored by synthesis, warning
endmodule

