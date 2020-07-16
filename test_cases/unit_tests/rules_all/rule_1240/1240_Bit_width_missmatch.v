module test;
wire [3:0] a;
wire [2:0] b;
assign a = b; //warning on "a" and "b"
endmodule

