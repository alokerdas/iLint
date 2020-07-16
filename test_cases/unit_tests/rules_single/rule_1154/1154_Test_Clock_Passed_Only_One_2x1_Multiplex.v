module top(q,d,c1,c2,c3,sel,set,reset);
output q;
input d,c1,c2,c3,sel,set,reset;
reg q;
wire clk;
wire c;
wire c4;
buf buffer(c,c1);
assign clk=sel?c:c4;
assign c4=sel?c2:c3;
always @(posedge clk or posedge set or negedge reset)
if(~reset)q=1'b0;
else if(set)q=1'b1;
else q=d;
endmodule

