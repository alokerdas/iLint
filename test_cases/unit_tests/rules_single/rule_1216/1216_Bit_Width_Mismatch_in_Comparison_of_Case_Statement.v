module test;
reg [8:0] out, b;
reg [1:0] a;
reg [1:0] se;
always @( a or b )
 case (a[1:0])
  2'b00, 2'b01: out = 1;
  2'b10:        out = (b << 2) + 1;
  2'b11:        out = b << 6;
  3'b100:        out = b << 4;
 endcase
endmodule

