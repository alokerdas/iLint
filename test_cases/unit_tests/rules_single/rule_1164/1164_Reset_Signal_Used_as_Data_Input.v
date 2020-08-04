module test (.py(y), .co(count), .cl(clock), .re(reset));
input clock, reset;
output [8:0] count;
reg [8:0] count;
output y;
reg y;
always @(posedge clock or posedge reset)
begin
if (reset)//"reset" is reset signal
count = 0;
else
count = count + 1;
end
always @(posedge clock)
y = reset; //warning here, "reset" is used as data input
endmodule
