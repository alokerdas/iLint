module test (clk1, clk2, en1, en2, d1, d2, q, qq);
input clk1, clk2, en1, en2, d1, d2;
output q, qq;
wire clk1, clk2, d1, d2, q, en1, en2;
reg q1, q2;
always @(posedge clk1)
q1 = d1;
always @(posedge clk2)
q2 = d2;
assign q = q1 ? en1 : 'bZ;
assign qq= q2 ? en2 : 'bZ;
endmodule
