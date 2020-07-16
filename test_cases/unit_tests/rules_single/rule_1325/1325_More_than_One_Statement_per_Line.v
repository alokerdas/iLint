module test(c, mem1, assignA, assignB, assignC, assignD);
input [7:0] c;
reg [7:0] mem [0:1023]; input mem1;
output [7:0] assignA, assignB, assignC, assignD;
assign assignA = c; assign assignB = c + 1; 
assign assignD = c + 3; assign assignC = c;
endmodule
