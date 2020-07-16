module test;
 reg a, b, c;
 initial begin
   a=b;
 end
 initial begin
   a=c; //"a" has been assigned in more than one block, warning
 end
endmodule

