module test (clock, data, y);
input clock;
input data;
output y;
reg y;
always @(posedge clock)
		y = data; 
endmodule

