module test (clk, datain, dataout);
 input clk, datain;
 output dataout;
 reg dataout;
 always @( posedge clk )
   dataout = datain; //warning on 'dataout', 'dataout_r' recommended
endmodule

