module test (y1, y2, data, clock, preset, clear);
input data, clock, preset, clear;
output y1, y2;
reg y1, y2;
always @(posedge clock or posedge preset)
//asynchronous set signal "preset"
begin: forset
if (preset)
y1 = 1;
else
y1 = data;
end
always @(posedge clock)
begin
if (preset)//synchronous set "preset", mixed
y2 = 1;
else
y2 = data;
end
endmodule
