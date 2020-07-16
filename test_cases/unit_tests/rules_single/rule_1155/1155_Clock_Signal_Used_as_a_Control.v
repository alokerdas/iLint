module test (clock2, data, y1, y2);
input clock2, data;
output y1, y2;
reg y1, y2;
always @(posedge clock2) //"clock2" is used as clock
y1 = data;
always @( data or clock2 )
if ( clock2 ) //warning
y2 = data;
else
y2 = ~data;
endmodule

