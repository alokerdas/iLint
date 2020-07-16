module smp(INA,F);
 input[7:0] INA;
 output       F;
 reg F;
 integer I;
  always @(INA) begin : loop
     I= -1;
     for (I = 0; I <= 7; I = I + 1)
      F = INA[I];
  end
endmodule

