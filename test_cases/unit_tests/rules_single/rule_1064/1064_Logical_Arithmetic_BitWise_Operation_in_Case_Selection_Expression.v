module smp(zo,sel1,sel2,clk);
 input clk;
 input[1:0] sel1,sel2;
 output[3:0] zo;
 reg [3:0] zo;
 always@(clk)
   case(sel1&sel2)
    2'b00: zo = 4'b0001;
    2'b01: zo = 4'b0010;
    2'b10: zo = 4'b0100;
    2'b11: zo = 4'b1000;
    default: zo = 4'b0000;
   endcase
endmodule

