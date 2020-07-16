module test;
 wire [7:0] regA;
 reg signed [7:0] regS;
 always @( regA )
  regS = regA; //warning here
endmodule

