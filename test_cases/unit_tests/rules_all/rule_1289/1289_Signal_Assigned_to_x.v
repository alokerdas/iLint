module smp(zo,fout,sel,res,ck,fin);
 input[1:0] sel;
 input        res, ck, fin;
 output[3:0] zo;
 output        fout;
 reg [3:0] zo;
 reg fout;
 parameter D1=1;
 always@( sel )
   case(sel)
     2'b00: zo = 4'b0001;
     2'b01: zo = 4'b0010;
     2'b10: zo = 4'b0100;
     default: zo = 4'bxx00; // Warning Here
   endcase
 always@( negedge res or posedge ck )begin
   if( ~res )
     fout <= #D1 1'b0;
   else if(zo==4'b0100)
     fout <= #D1 1'bz; // Warning Here
   else
     fout <= #D1 fin;
 end
endmodule

