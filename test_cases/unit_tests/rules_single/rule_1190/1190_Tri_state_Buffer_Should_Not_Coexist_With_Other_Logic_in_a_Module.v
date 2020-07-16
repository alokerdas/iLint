module smp1(rden, wren, data, tri_out1, cnt1);
 input rden, wren, data;
 output tri_out1;
 output cnt1;
  bufif1 BUF( tri_out1 , rden , data );
  and AA( cnt1 , tri_out1 , wren); //warning here
endmodule





