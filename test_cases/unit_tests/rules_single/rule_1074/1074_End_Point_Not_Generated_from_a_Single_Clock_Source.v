module test (clk1, clk2, clk, d1, d2, q, qq, qqq);
input clk1, clk2, clk, d1, d2;
output q, qq, qqq;
wire temp, temp1, temp2;
reg q1, q2, q, qq, qqq;
always @(posedge clk1)
q1 = d1;
always @(posedge clk2)
q2 = d2;
assign temp2 = q1 & q2;
assign temp1 = temp2;
assign temp = temp1;
///// FOR LPM /////
always @(negedge clk)begin
qq = temp; // "temp" comes from more than one clock source.
q = q1 & q2; // out put of "q1" & "q2" comes from more than one clock source.
end
///// FOR Signal /////
always @(q1 or q2)begin
qqq = temp; // "temp" comes from more than one clock source.
end
endmodule
