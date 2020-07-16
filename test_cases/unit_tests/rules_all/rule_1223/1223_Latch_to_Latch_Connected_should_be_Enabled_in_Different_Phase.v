module top (a3);
output a3;
wire [1:0] tri_,latch;
wire c;
wire a1;
sub s1(latch[0], c, a2);
assign a1=a2;
sub s2(latch[0], a1, a3);
endmodule
module sub (latch , c, q);
input latch, c;
output q;
wire latch, c;
reg q;
always @(latch or c) begin
if(latch)
begin
q=c;
end
end
endmodule
