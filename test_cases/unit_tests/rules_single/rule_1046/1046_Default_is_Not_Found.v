module test (sel,a,c);
 input [1:0] a ;
 input [1:0] sel;
 output [1:0] c;
 reg [1:0] c;
 `define ZERO 0
 `define ONE 1
 `define TWO 2
 `define THREE 3
 always @ (a or sel)
   case (sel) //warning here, default item is lost
             //If sel signal is X or Z , it may cause problem
   `ZERO: c = 2'b00;
   `ONE : c = 2'b01;
   `TWO : c = a;
    //default: c= 2'b11;
   //`THREE: c= 2'b11;
   endcase
endmodule

