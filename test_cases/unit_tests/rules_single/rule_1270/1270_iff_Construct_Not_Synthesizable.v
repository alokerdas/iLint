module test;
reg rst, d;
reg q;
always @ (d iff rst == 0) // Worning Here
begin
if ( ~rst )
q = 1'b0;
else
q = d;
end
endmodule
