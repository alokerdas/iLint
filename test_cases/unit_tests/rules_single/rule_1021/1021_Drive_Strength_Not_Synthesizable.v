module test (a, b);
 output a;
 input b;
 assign (strong1,pull0) b = a; //"strong1,pull0" non-synthesizable
endmodule

