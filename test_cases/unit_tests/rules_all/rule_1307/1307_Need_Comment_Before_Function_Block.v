module testalways(a, b, c);
input a, b;
output c;
reg c;
always@(a or b) //warning
c = a & b;
endmodule

