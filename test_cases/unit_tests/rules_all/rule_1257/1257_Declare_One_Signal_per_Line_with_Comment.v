module test (clock,data,y);
input clock,data;
output y;
reg y;
wire a,b; // warning here
always @(posedge clock)
y=data;
endmodule

