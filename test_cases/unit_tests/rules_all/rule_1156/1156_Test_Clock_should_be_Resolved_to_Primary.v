module top(q,d1,d2,ctrl,clk,reset);
output q;
input d1,d2,ctrl,clk,reset;
wire gen_clk;
reg q;
reg o;
always @(posedge clk or negedge reset)
if(~reset)o=1'b0;
else o=d1;
assign gen_clk=o&ctrl;//report here
always @(posedge gen_clk)
q=d2;
endmodule

