module test (a, d);
input a;
output d;
reg b, d;
wire c;
always @(a or c)
begin
b = a;
d = c;
end
assign c = a & b;
endmodule

