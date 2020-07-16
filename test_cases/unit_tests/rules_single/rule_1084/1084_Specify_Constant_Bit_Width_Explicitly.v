module smp(A,B,C);
 input[6:0] A,B;
 output[6:0] C;
 reg[6:0] C;
 always@(A or B or C)
  case( A )
   7'd0 : C = B;
   7'd1 : C = 'b000_0010;
   7'd2 : C = 'b000_0100;
   7'd3 : C = 'b000_1000;
   7'd4 : C = 'b001_0000;
   default : C = 'b111_1111;
  endcase
endmodule

