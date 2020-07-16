module test;
wire clk2, clk1, d, set; //2 clk sources
reg q;
always @(posedge clk2) begin
if (set) q <= 1'b1;
else q<=d;
end
always @(posedge clk1) begin
if (set) q<= 1'b1;
else q<=d;
end
endmodule

