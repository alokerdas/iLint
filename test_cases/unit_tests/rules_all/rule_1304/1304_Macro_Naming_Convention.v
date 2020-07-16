`define add(a,b) a + b
`define NAND(delay) nand #(delay)
`ifdef DEFed
`define Def1 0
`else
`define Def2 0
`endif
`undef DEFed
//warning
//OK
//warning
//warning
//warning
//warning
module test;
endmodule

