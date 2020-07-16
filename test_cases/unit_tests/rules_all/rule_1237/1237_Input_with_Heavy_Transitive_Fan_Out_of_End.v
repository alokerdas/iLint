module test (a,clk, r1, r2, r3, r4, r5);
input a, clk;
output r1, r2, r3, r4, r5;
wire a, clk;
reg r1, r2, r3, r4, r5;
always @(posedge clk)
r1=a;
always @(posedge clk)
r2=a;
always @(posedge clk)
r3=a;
always @(posedge clk)
r4=a;
always @(posedge clk)
r5=a;
endmodule

