module test (a,b,c);
 input a,b;
 output[2:0] c;
 reg [2:0] c;
 always@(b)
 begin
   if(b)
     if (a)
       c= 1;
     else
      c= 2;
 end
endmodule

