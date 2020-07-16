module test (dataout, s, datain,clk);
 input clk;
  parameter WIDTH = 4;
  output dataout;
  input s;
  input [WIDTH-1:0] datain;
  reg dataout;
  always @(clk)//s or datain)
     begin
     if (s == 'bz) //warning
       dataout = datain[0];
     else
       if (s == 'bx) //warning
          dataout = datain[1];
       else
          if (s == 'b0)
            dataout = datain[2];
     else
            dataout = datain[3];
     end
endmodule

