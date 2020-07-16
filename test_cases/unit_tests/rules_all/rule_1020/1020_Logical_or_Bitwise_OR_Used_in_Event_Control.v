module test (clk, rst, set, Data, q1, q2);
 input clk, rst, set;
 input Data;
 output [1:0] q1, q2;
 reg [1:0] q1, q2;
 //should be always@(posedge clk or posedge rst)
 always@(clk | rst)
   begin
    if (rst)
      q1 <= 0;
    else
      q1 <= Data;
   end
endmodule
