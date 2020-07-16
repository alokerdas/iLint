module test;
`ifdef BYPASS_VLD_FIFO //"`ifdef" directive unrecommended, warning
FLAG_BBE = FLAG_BBE_0 ;
`else
//FLAG_BBE = DIS_VLD | FLAG_BBE_0 ;
`endif
endmodule

