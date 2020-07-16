module test (clk, rst, d, q);
 input clk, rst;
 input d;
 output q;
 reg q;
 always @( posedge clk or rst ) //warning
   if( rst )
     q <= 1'b0;
   else
     q <= d;
endmodule

