module test (out, clk, in, cas);
 input clk, in, cas;
 output out;
 reg out;
 always @(clk)
   case (cas)
    1'b0: out = !in;
    1'b1: out = in;
    1'bz: out = 1; //warning on "1'bz"
    1'bx: out = 0; //warning on "1'bx"
    default: out = 1;
   endcase
endmodule

