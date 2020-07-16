module smp(A,B,C,G,H,Z);
input  [1:0]A, B, C,G,H;
output  [1:0]Z;
reg  [1:0]Z;
 always@( A or B or C or G or H )
   case ({G,H})
    2'b00 : Z <= A;
    2'b01 : Z <= B;
    2'b10 : Z <= C;
    2'b11 : Z <= -1;
   endcase
endmodule

