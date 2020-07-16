module smp1(rden, wren, data, tri_out1, cnt1, iiii2);
 input rden, wren, data, iiii2;
 output tri_out1;
 output cnt1;
  assign tri_out1 = ( rden ) ? data : iiii2;
  assign cnt1 = tri_out1 & wren; //warning here
endmodule

