module test (out, in);
 output [7:0] out;
 input in;
 wire [7:0] out;
 integer i;
 assign out = {in, i}; //warning
endmodule

