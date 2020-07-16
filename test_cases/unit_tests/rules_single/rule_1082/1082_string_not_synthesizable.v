module test (c,a,b);
output c;
input a,b;
reg c;
reg [8*14:1] strvar;
always @(a or b)
begin
if (a == b)
begin
c = 'b1;
strvar = "are equal."; //"are equal." non-synthesizable, warning
end
else
begin
c = 'b0;
strvar = "are not equal."; //"are not equal."
//non-synthesizable, warning
end
end
endmodule

