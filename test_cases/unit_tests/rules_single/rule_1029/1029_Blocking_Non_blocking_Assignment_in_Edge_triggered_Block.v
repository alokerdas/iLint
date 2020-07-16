module test;
 reg clock;
 reg a,b,c;
 always @(posedge clock)
  begin
   a = b;
   c <= a; //choose BLOCKING, IGNORE_DEPEND;
        //block assignment in edge-trigger block will cause
        //mismatch between pre-synthesis and
        //post-synthesis simulation
  end
endmodule

