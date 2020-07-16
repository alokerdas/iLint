module initval;
 reg clock, reset;
 wire [8:0] count;
 initial
   begin
   clock=0;
   reset=0;
   $monitor($time,,,"count=%d,reset=%d",count,reset);
   #200 $finish;
   end
 always
   #4 clock=!clock;
 //test cc (clock, reset, count);
endmodule

module second_mod;
 reg clock, reset;
 wire [8:0] count;
 initial
   begin
   clock=0;
   reset=0;
   $monitor($time,,,"count=%d,reset=%d",count,reset);
   #200 $finish;
   end
 always
   #4 clock=!clock;
 //test cc (clock, reset, count);
endmodule

