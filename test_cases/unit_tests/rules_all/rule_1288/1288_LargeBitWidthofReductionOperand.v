module test;
 reg [39:0] a,d;
 wire b,e;
 wire [31:0] f;
 assign b = &a; // Warning on a;
 assign e = |f; // No Warning
endmodule

