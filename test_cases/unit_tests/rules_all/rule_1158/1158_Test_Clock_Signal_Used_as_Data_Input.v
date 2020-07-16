module top(q1,q2,d,clk1,clk2,reset);
output q1,q2;
input d,clk1,clk2,reset;
reg q1,q2;
always @(posedge clk1 or negedge reset)
if(~reset)q1=1'b0;
else q1=d;
always @(posedge clk2)
q2=clk1&d;//violation location: line no 13.
endmodule
