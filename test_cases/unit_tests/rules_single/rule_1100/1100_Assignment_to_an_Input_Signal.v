module test (a, b, c);
 input [7:0] a, b;
 output [7:0] c;
 reg [7:0] c;
 wire [7:0] a;
 assign a[2] = 0; //warning on "a[2]", input signal is assigned
// always @(a)
  // c = b;
endmodule

