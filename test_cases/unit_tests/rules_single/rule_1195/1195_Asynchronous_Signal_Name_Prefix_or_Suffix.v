module test (clk,rst_pa,count);
input clk,rst_pa;
output[7:0] count;
reg[7:0] count;
reg[7:0] nextCount;
always @(posedge clk or posedge rst_pa)
if (rst_pa)
count <= 0;
else
count <= nextCount;
endmodule
