module smp(INA,F);
 input[7:0]   INA;
 output       F;
 reg F;
 integer I;
  always @(INA) begin : loop
      for (I = 0; I <= 7; I = I + 1)
        F = INA[I] + 1'b1;
  end
endmodule

