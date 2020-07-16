module test(clk,data1,data2,y1,y2);
input clk,data1,data2;
output y1,y2;
reg y1,y2;
always_comb begin
y1 = data1;
@(y1)
y2 = data2;
end
endmodule
