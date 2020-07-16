module top;
integer ns, ps;
reg en;
always
begin
casex ({1'b0,1'b?,1'b1 }) //waning here, '?' is used in casex
3'b?xz: ns = 1;
3'b00x: ns = 2;
default: ns = 0;
endcase
end
always
begin
casez (en? 1'bz: 1'bz) //warning here, 'z' is used in casez
1'b?: ps = 1;
1'b0: ps = 2;
default: ps = 0;
endcase
end
endmodule
