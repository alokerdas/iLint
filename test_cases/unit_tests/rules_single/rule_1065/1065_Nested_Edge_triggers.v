module initval;
   reg clock,reset;
    wire [8:0] count,y;
    initial
      begin
       $monitor($time,,,"clock=%d,count=%d,reset=%d,y=%d",clock,count,reset,y);
       clock=0;
       reset=0;
       #20 reset=1;
       #10 reset=0;
       #50 $finish;
     end
   always
     #4 clock=!clock;
   test cc (y,clock,reset,count);
  endmodule
module test (y,clock,reset,count);
 input clock,reset;
 output [8:0] count,y;
 reg [8:0] count,y;
 initial
   count<=0;
 always @(posedge clock)
   begin
     if ( reset )
        count = 0;
    else
        count= count + 1;
     @(posedge clock)
        y=count;
   end
endmodule

