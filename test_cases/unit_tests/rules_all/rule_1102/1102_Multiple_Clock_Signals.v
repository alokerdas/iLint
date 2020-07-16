module test (clock1,clock2,reset,count,data,y);
input clock1, clock2, reset, data;
output [8:0] count;
reg [8:0] count;
output y;
reg y;
always @(posedge clock1 or negedge reset)//clock signal "clock1"
begin
if (~reset)
count = 0;
else
count= count + 1;
end
always @(posedge clock2)//warning on "clock2", another clock signal
begin
if (~reset)
y = 0;
else
y = data;
end
endmodule
