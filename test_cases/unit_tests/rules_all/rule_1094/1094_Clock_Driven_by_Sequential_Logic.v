module test (count, reset, clock, data);
input clock, reset, data;
output [8:0] count;
reg [8:0] count;
reg qc;
always @(posedge clock)
qc = data; //"qc" is output of register, warning on "qc"
always @(posedge qc or negedge reset)
begin //qc is used as clock signal and drived
//by above sequential logic
if (~reset)
count = 0;
else
count= count + 1;
end
endmodule

