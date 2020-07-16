module test (s, o);
 input s;
 output o;
 reg o;
 always @( s ) begin : P_0123456789012345
              //warning on "P_0123456789012345"
   o = s;
 end
endmodule

