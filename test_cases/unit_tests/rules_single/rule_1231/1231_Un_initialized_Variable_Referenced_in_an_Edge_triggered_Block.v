module test (clk, data, count);
  input clk,data;
  output count;
  reg     count;
  reg     t;
  //initial
  // t = 1;
  //if remove the comment of initial block,
  //warning 27125 on "t" is gone
  always @( posedge clk )begin
    count = t & data; //variable "t" is referenced un-initialized
  end
endmodule

