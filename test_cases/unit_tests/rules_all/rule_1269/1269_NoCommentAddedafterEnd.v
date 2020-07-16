module test (sel,a,b,c);
input a,b,sel;
output c;
reg c;
always @ (a or b or sel)
begin
case(sel)
1'b0: c = a & b;
1'b1: c = a | b;
default: c = 1'b0;
endcase //no warning here, warning if "ENDCASE" is in argument
end
//no warning here, warning if there is no this comment
endmodule

