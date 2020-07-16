module test (q, clk, reset, d);
 output q;
 input clk, reset, d;
 reg q;
 integer a,b;
 wire clk, reset, d;
 always @(posedge clk)
   q <= d;
 always @(reset)
    assign q = 1'bx;
endmodule

