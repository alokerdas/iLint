module test;
 wire clk,rst1_n;
 wire rst2, rst_test_n;
 reg [8:0] count;
 always @(posedge clk or negedge rst1_n) begin
  if (~rst1_n) //warning here
    count = 0;
  else
    count = count + 1;
 end
 always @(posedge clk or negedge rst2) begin
  if (rst2) //no warning
    count = 0;
  else
    count = count + 1;
 end
 always @(posedge clk or negedge rst_test_n) begin
  if (~rst_test_n) //no warning
    count = 0;
      else
    count = count + 1;
 end
endmodule

