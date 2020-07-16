module test (a,b,c,SADDR,E21_REG_BPP,y,SOFFSET);
input a,b,c;
input [03:00] SADDR;
input [01:00] E21_REG_BPP;
output y;
output [03:00] SOFFSET;
reg [03:00] SOFFSET;
assign y =
// synopsys translate_off
a | b |
// synopsys translate_on
c;
always @(E21_REG_BPP)
case (E21_REG_BPP[01:00])
2'b00 : SOFFSET[03:00] <= SADDR[03:00];
2'b01 : SOFFSET[03:00] <= {SADDR[02:00], 1'b0};
2'b11 : SOFFSET[03:00] <= {SADDR[01:00], 2'b0};
//synopsys translate_off
default : SOFFSET[03:00] <= 0;
//synopsys translate_on
endcase
endmodule

