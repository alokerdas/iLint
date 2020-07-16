module smp(RES,CK,FIN1,FIN2,FOUT1,FOUT2);
input
RES,CK,FIN1,FIN2;
output FOUT1,FOUT2;
reg
FOUT1,FOUT2;
always@(negedge RES or posedge CK)
if(~RES)
FOUT1 <= 1'b0;
else
begin
FOUT1 <= FIN1;
FOUT2 <= FIN2;
end
endmodule
