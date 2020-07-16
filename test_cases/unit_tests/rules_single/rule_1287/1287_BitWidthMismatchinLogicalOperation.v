module smp(A,B,X,Y,C,Z);
 input X;
 input [1:0]Y;
 input [1:0] A;
 input [2:0] B;
 output C,Z;
 assign C = ( A || B ) ? 1'b1 : 1'b0; //warning on (A && B) here
 assign Z = ( X && Y ) ? 1'b1 : 1'b0; //warning on (X && Y) here
endmodule

