module test(tri_out1,data,a,b);
 input data,a,b;
 output tri_out1;
 wire tri_out1, rden, data;
 assign tri_out1 = ( rden ) ? data : 1'bz;
 and AA(rden,a,b); //warning here
 //not AA(rden,b);
endmodule

/*module smp1(rden, wren, data, tri_out1, cnt1);
 input rden, wren, data;
 output tri_out1;
 output cnt1;
  assign tri_out1 = ( rden ) ? data : 1'bz;
  assign cnt1 = tri_out1 & wren; //warning here
endmodule
*/
