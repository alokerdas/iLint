module TriState(y,en,data,Q,G,DATA);
 output y,Q;
 input en,data,G,DATA;
 reg y,Q;
 always @(en or data)
   if(en)
     y = data;
   else
     y = 1'bz;
 always @(G or DATA)
   if(G)
     y <= DATA;
endmodule

