module test;
reg ck,reset;
wire [8:0] count;
block cc (.clk(ck),.RESET(reset),.counti(count)); //warning on clk,
endmodule

module block(.clk(clock),.RESET(reset),.counti(count)); //warning on clk
input clock,reset;
output [8:0] count;
reg [8:0] count;
initial
count<=0;
always@( posedge clock or negedge reset) begin
if ( ~reset )
count = 0;
else
count= count + 1;
end
endmodule

