module test(clk, d, q);
input clk, d;
output q;
wire clk, d;
reg q1, q;
always @(posedge clk)
  q1 = d;
always @(negedge clk)
  q = q1;
endmodule

