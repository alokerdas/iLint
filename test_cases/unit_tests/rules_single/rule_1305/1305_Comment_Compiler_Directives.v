`include "lib.v"
`ifdef FOR_SIMULATION //abc
`define SIMULATION 1
`else /*cba*/
`define SIMULATION 0 //xyz
`endif
module test;
endmodule
