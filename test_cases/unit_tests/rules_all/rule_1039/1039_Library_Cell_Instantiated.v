module smp(dout,ck,res,din);
input ck,res,din;
output dout;
reg douti;
parameter D1 = 1;
always @(posedge ck or negedge res)
     if ( ~res )
        douti <= #D1 1'b0;
     else
        douti <= #D1 din;
JANIV OBUF (.A(douti), .Z(dout));
endmodule
`celldefine
module JANIV(A, Z);
input A;
output Z;
assign Z = A;
endmodule
`endcelldefine

