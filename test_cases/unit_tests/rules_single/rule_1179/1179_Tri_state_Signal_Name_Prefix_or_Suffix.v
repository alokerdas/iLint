module smp1(rden, wren, data, tri_out1, cnt1);
 input rden, wren, data;
 output tri_out1;
 output cnt1;
  assign tri_out1 = rden ? data : 1'bz;
  assign cnt1 = tri_out1 & wren; 
endmodule

