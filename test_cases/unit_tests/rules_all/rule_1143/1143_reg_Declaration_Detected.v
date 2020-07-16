module test (clk,count);
 input clk;
 output [8:0] count;
 reg [8:0] count;
 reg [8:0] nextCount;
 always @(posedge clk)
    count = nextCount;
endmodule

