module test (clock,VMA,R_W,addr,data);
input clock;
input VMA;
input R_W;
input [1:0] addr;
inout [1:0] data;
reg [1:0] macroram [5:0];
reg [1:0] dataout;
assign data = R_W ? dataout : 8'hz;
always @(posedge (clock & VMA))
begin
if (R_W == 1)
dataout=macroram[addr];
macroram[addr]=data; //"macroram" is read and written at same time
end
endmodule
