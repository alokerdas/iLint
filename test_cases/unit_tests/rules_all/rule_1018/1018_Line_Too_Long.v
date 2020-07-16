module test;
      reg sel;
      reg [7:0] a,b,c,d,e,f,g;
      initial
      begin//:B
$monitor($time,,,,,,,"sel=%d,a=%d,b=%d,c=%d,d=%d,e=%d,f=%d,g=%d",sel,a,b,c,d,e,f,g);
   //do not type a line so long.
      a=12;
      b=13;
      c=14;
      e=15;
      f=16;
      g=17;
      sel=1;
      #200 $finish;
    end
    always
    begin//:A
      # 20 sel = ~sel;
    end
  endmodule

