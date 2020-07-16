module test;
 function [12:0] mu; //warning on "mu"
 input [12:0] a;
 input [15:0] b;
 begin: serialMult
 reg [5:0] mcnd,mpy;
   mpy = b[5:0];
   mcnd = a[5:0];
   mu=0;
   repeat(6) begin
    if(mpy[0])
      mu = mu + {mcnd,6'b000000};
    mu = mu >> 1;
    mpy = mpy >> 1;
   end
 end
 endfunction
endmodule

