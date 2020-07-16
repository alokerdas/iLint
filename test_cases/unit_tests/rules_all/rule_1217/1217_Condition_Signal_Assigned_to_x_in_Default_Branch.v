module smp(zo,sel);
 input[1:0] sel;
 output[3:0] zo;
 reg [3:0] zo;
 always@( sel )
   case(sel)
    2'b00: zo = 4'b0001;
    2'b01: zo = 4'b0010;
    2'b10: zo = 4'b0100;
    default: zo = 4'bxxxx;
   endcase
endmodule

