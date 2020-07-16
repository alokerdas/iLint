module test;
reg clk, rst, d;
reg q;
always @ (changed rst, posedge clk or d) // Worning Here
begin
if ( ~rst )
q = 1'b0;
else
q = d;
end
endmodule
