module test (clock, clock2, reset, count, d);
input clock, clock2, reset, d;
output [8:0] count;
reg [8:0] count;
reg a;
always @(posedge clock or negedge reset) //"clock" is used as clock
begin
if (~reset)
count = 0;
else
count= count + 1;
end
always @(posedge clock2)
if (clock) //warning
a=0;
else
a=d;
endmodule

