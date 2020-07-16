module test (data, count);
  input data;
  output count;
  reg     count;
  reg     t;
  //initial
  // t = 1;
  //if remove the comment of initial block, violation will disappear.
  always @( data ) begin
    count = t && data; //Here "t" is referenced un-initialized
  end
endmodule

