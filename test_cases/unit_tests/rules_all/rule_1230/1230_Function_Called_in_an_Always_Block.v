module smp(CK,A,B,C,D,G,H,ZF);
input CK;
input A,B,C,D,G,H;
output ZF;
reg ZF;
function ZFC;
input A,B,C,D,G,H;
 begin
   case ({G,H})
    2'b00 : ZFC = A;
    2'b01 : ZFC = B;
    2'b10 : ZFC = C;
    2'b11 : ZFC = D;
    default : ZFC = 1'bx;
   endcase
 end
endfunction
always@( posedge CK )
   ZF <= ZFC ( A,B,C,D,G,H );
endmodule

