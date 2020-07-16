module test (out, sel, tmp, data);
 output out;
 input [2:0] sel, data;
 input [1:0] tmp;
 reg out;
 always @(sel or data or tmp)
   casex (sel) // Synopsys full_case
    3'bx0: out = data[0];
    3'b01: out = data[1];
    tmp: out = data[2]; //warning
    default: out = 0;
   endcase
endmodule

