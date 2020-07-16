module test (clock, reset, control, y);
 input clock, reset, control;
 output [2:0] y;
 reg [2:0] y;
 parameter st0 = 0; //warning on st0, p_st0 is recommended
 reg [1:0] current, next;
endmodule

