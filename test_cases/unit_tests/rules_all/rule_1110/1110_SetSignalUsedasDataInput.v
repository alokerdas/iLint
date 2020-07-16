module test (clock2, set, data, y1, y2);
 input clock2, data, set;
 output y1, y2;
 reg y1, y2;
 always @(posedge clock2 or negedge set )
   if (~ set )//"set" is used as set signal
     y1 <= 1'b1;
   else
     y1 <= data;
  always @(posedge clock2)
   y2 = set;//"set" is used as data input, warning on "set"
endmodule

