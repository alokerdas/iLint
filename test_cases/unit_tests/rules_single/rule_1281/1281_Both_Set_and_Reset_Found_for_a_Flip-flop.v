module test(clk,set,reset,set1,reset1,enable,D,Q1,Q2);
input clk,set,reset,set1,reset1, enable;
input D;
output Q1,Q2;
reg Q1,Q2;
always @(posedge clk or negedge reset or posedge set)
begin
if (set)
Q1 <=1'b1;
else if (!reset) //violation here
Q1 <= 1'b0;
else
Q1 <= D;
end
always @(posedge clk or negedge reset1 or posedge set1)
begin
if (set1)
Q2 <= 1'b1;
else if(!reset1)
Q2 <= 1'b0; //violation here
else
Q2 <= D;
end
endmodule
