module test (clock, reset, count, clk_out);
input clock, reset;
output [8:0] count;
output clk_out;
reg [8:0] count;
always@(posedge clock or negedge reset)//"clock" is used as clock
begin
if (~reset)
count = 0;
else
count= count + 1;
end
assign clk_out = clock; //warning
endmodule
