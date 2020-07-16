module test (clock2, clock1, data, y1, y2);
input clock2, clock1, data;
output y1, y2;
reg y1, y2;
always @(posedge clock1)
y1 = clock2; //warning
always @(posedge clock2)
y2 = data;
endmodule
