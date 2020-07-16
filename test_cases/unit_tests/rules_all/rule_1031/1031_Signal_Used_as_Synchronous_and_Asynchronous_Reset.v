module test (y1, y2, data, clock, preset, clear);
input data, clock, preset, clear;
output y1, y2;
reg y1, y2;
always @(posedge clock or posedge clear or posedge preset)
//asynchronous set signal "preset"
begin: forset
if (clear)
y1 = 0;
else
if (preset)
y1 = 1;
else
y1 = data;
end
always @(posedge clock)
begin
if (clear)
y2 = 0;
else
if (preset)//synchronous set "preset", mixed
y2 = 1;
else
y2 = data;
end
endmodule

