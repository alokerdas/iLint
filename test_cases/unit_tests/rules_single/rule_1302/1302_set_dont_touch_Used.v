module smp(out1, out2, in, clk);
output out1, out2;
input in, clk;
//synopsys set_dont_touch FF1
FF1 myFF1(.Din(in), .Clk(clk), .Q(out1), .QB(out2));
endmodule
`celldefine
module FF1(Din, Clk, Q, QB);
input Din, Clk;
output Q, QB;
reg Q, QB;
always @(posedge Clk)
begin
Q = Din;
QB = ~Din;
end
endmodule
`endcelldefine

