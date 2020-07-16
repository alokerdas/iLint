module smp(RES1,RES2,CK,FIN,FOUT);
 input RES1,RES2,CK,FIN;
 output FOUT;
 reg     FOUT;
 parameter D1 = 1;
 always@(negedge RES1 or negedge RES2 or posedge CK)
   if(RES1 || !RES2)
     FOUT <= #D1 1'b0;
   else
     FOUT <= #D1 FIN;
endmodule

