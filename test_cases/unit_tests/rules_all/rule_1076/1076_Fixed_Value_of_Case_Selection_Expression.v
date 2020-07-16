module smp(zo,sel);
 input[3:0] sel;
 output[3:0] zo;
 reg [3:0] zo;
 always@( sel )
   case(1'b1)
    sel[0]: zo = 4'b0001;
    sel[1]: zo = 4'b0010;
    sel[2]: zo = 4'b0100;
    sel[3]: zo = 4'b1000;
    default: zo = 4'b0000;
   endcase
endmodule

