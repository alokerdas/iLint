module smp(INA,INB,F);
 input[7:0]   INA,INB;
 output       F;
 reg F;
 integer I;
  always @(INA or INB) begin : loop
      for (I = 0; I <= 7; I = I + 1)
        F = INA[I]^INB[I];
  end
endmodule

