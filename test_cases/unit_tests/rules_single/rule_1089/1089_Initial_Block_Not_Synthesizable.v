module test;
reg a, b;
initial //non-synthesizable, warning
begin
a = 0;
b = 1;
end
endmodule

