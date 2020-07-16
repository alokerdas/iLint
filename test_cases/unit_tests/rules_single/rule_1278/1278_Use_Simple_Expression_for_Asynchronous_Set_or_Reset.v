module test2(d1, clk, R, d, q, d2);
input d1, d2, clk, R, d;
output q;
reg q;
always @( posedge clk or posedge R) begin
if ( | R ) //not a simple condition
q = 0;
else
q = d;
end
always @( posedge clk or posedge R) begin
if ( & R ) //not a simple condition
q = 0;
else
q = d;
end
always @( posedge clk or posedge R) begin
if (R == 0) //not a simple condition
q = 0;
else
q = d;
end
always @ (posedge clk or negedge R)
begin
if (R) // No Warning
q = 0;
else
q = d;
end
endmodule
