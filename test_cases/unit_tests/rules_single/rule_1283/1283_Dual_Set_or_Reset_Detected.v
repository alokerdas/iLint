module test (clock, ctrl1, ctrl2, q, q1, d);
 input clock, ctrl1, ctrl2;
 input d;
 output q, q1;
 reg q, q1;
 always @( posedge clock or negedge ctrl1 or negedge ctrl2 )
   if ( !ctrl1 )
     q <= #1 1'b0; //ctrl1 is treated as reset
   else if ( !ctrl2 )
     q <= #1 1'b0; //ctrl2 is treated as reset, warning here
   else
     q <= d;
endmodule

