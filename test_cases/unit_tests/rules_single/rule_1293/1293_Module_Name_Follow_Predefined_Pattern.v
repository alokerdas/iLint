module mGTECH1oo1(i, o);
 input i;
 output o;
endmodule
module mGTECH1(i, o);
 input i;
 output o;
 mGTECH1oo1 inst_2 (i, o); //warning with the pattern .*GTECH.*
endmodule
module GTECHtest; // No warning Here as Top Module
 mGTECH1 inst_1 (i, o); //warning with the pattern .*GTECH.*
endmodule

