module test;
 reg in;
 reg a,o;
 always @(in)
  begin
   a <= in;
   o <= a; //choose NONBLOCKING, IGNORE_DEPEND;
          //non-block assignment in combinational block will
          //cause mismatch between pre-synthesis and
          //post-synthesis simulation
  end
endmodule

