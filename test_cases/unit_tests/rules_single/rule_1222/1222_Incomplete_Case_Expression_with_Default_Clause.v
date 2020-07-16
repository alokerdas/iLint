module test;
     wire [1:0] a;
     reg b;
     always @(a)
      casez (a[1:0]) //warning here, 2'b11 is not covered but default
        2'b00: b = 1'b1;
        2'b01: b = 1'b0;
        2'b10: b = 1'bx;
       default: b = 1'bz;
    endcase
   endmodule

