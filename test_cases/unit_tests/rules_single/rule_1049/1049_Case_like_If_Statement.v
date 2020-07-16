module test (out, sel, in);
 parameter SEL_WIDTH = 2, D_WIDTH = 4;
 output out;
 input [SEL_WIDTH-1:0] sel;
 input [D_WIDTH-1:0] in;
 reg out;
 always @(sel or in)
   if (sel == 0) //this "if" statement should be replaced with
                //case statement, which is parallel process;
                //warning
     out = in[0];
   else
     if (sel == 1)
        out = in[1];
     else
        if (sel == 2)
          out = in[2];
        else
          out = in[3];
endmodule

