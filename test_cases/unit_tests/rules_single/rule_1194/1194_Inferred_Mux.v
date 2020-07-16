module test (q, c, a, b);
 input [1:0] a, b, c;
 output [1:0] q;
 reg [1:0] q;
 always@(a or b or c)
   if (a)
     q = b;
   else
     q = c; //a mux "c" inferred
endmodule

