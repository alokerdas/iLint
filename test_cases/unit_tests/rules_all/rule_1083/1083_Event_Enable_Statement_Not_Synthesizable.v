module test (clk, rst, d, q);
 input clk, rst, d;
 output q;
 reg q;
 event event1,event2;
 always @(posedge clk)
   if ( !rst )
     ->event1;//"->enent1" non-synthesizable, warning
   else
     ->event2;//"->enent2" non-synthesizable, warning
 always @event1
   q <= 0;
 always @event2
   q <= d;
endmodule

