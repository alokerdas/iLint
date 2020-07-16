module testini;
 reg [1:0] a, b, temp;
 wire [1:0] c;
 initial
 begin
   a = 2'b01;
   b = 2'b11;
   #50 a <= ~a;
   deassign temp; //"temp" is deassigned but never been assigned,
             //warning
   #100 $finish;
 end
endmodule

