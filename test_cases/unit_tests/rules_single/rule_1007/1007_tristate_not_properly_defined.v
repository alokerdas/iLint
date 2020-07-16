module Test (I1, i2, e, o);
  input I1, i2, e;
  output o;
  assign o = I1;
  assign o = e ? i2 : 1'bz; //warning on "o"
endmodule

