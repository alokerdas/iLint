module smp(A,B,C,D,S,W);
 input A,B,C,D,S;
 output W;
 reg W;
  always@( A or B or C or D or S )
   begin
    if( A )
      begin
        W = B;
      end
    case( S )
        1'b0 : W = C;
        1'b1 : W = D;
    endcase
   end
endmodule

