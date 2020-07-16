module top_27128_1( data, count );
input  data;
output count;
reg    count;
reg    t;
initial t = 1'b1; //Even "t" is initialized here, it is ignored.
                  //Because initial block is ignored by synthesizer.
always @( data ) begin
  count = t & data; //Here "t" has not been assigned before referenced.
                    //After synthesis, it is directly linked to '0',
                    //which cause mismatch between pre-synthesis and
                    //post-synthesis
end
endmodule

