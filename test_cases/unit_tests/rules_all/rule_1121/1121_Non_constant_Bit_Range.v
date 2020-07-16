module test (out, s, in);
 parameter S_WIDTH = 3;
 parameter DATA_WIDTH = 8;
 output out;
 reg out;
 input [S_WIDTH-1:0] s;
 input [DATA_WIDTH-1:0] in;
 always @(in)
   out = in[s]; //"s" is not a constant in bits selection, warning
endmodule

