module test (clk,b,c);
 input clk;
 input [7:0] b;
 output [7:0]c;
 reg [7:0]c;
 always @(clk) begin
   repeat(4'b0x10) //repeat zero times
    c=b;
 end
endmodule

