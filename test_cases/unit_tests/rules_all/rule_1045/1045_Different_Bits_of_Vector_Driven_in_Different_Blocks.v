module test (a, b, c);
 input a, b;
 output [2:0] c;
 reg [2:0] c;
 initial
    c[2] = 1;
 always@(a)
   c[0]=a;         //warning here
 always@(b)
   c[1]=b;         //warning here, "c[1]" and "c[0]" are used
               //in different always blocks
endmodule

