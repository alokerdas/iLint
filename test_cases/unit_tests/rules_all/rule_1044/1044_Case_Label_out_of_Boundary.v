/*module smp(A,B,C,G,H,Z);
input A,B,C,G,H;
output Z;
reg Z;
 always@( A or B or C or G or H )
   case ({G,H})
    2'b00 : Z <= A;
    2'b01 : Z <= B;
    2'b10 : Z <= C;
    2'b11 : Z <= -1;
    default : Z <= 1'bx;
   endcase
endmodule
*/
module test (Clock);
 input Clock;
 reg [3:0] state;
 reg [2:0] data;
 always@(Clock)
   begin
    case (state)
     16: data <= 3'b111; //warning
     15: data <= 3'b110;
     default: data <= 3'b000;
    endcase
   end
endmodule


