module test ( a, b, c, y, d );
input a, b, c, y;
output d;
reg d;
always @(a or b or c or y)
  if(a&b|c)     // warning here if TRI_ENABLE is selected
    d = y;
  else
    d = 1'bz;
endmodule

