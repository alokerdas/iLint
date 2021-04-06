module test;
reg [3:0] a;
wire [2:0] b;
always @(a)
  assign a = b; //warning on "a" and "b"
endmodule

