module test (q, clock, reset, d);
 output q;
 input clock, reset, d;
 reg q;
 wire clock, reset, d;
 parameter D_RQ = 1, D_CQ = 2;
 always @(posedge clock or negedge reset)
   if (~reset)
     #D_RQ q <= 0; //delay control "#D_RQ" may cause
               //non-blocking effect invalid, warning
   else
     #D_CQ q <= d; //delay control "#D_CQ" may cause
               //non-blocking effect invalid, warning
endmodule

