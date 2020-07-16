module test;
 wire [7:0] a;
 wire [7:0] b;
 wire [7:0] c;
 assign a = b * c; //warning on "b" and "c"
endmodule

