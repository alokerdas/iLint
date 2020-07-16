module test (clk, data, count);
  input clk,data;
  output count;
  reg     count;
  reg     t;
  initial
    t = 1'b1; //Even "t" is initialized here, it is ignored
               //Because initial block is ignored by synthesizer.
  always @( posedge clk )begin
    count = t & data; //Here "t" has not been initialized before
              //reference. After synthesis, "t" is directly linked
              //to '0', which cause mis-match between
              //pre-synthesis status and post-synthesis.
  end
endmodule

