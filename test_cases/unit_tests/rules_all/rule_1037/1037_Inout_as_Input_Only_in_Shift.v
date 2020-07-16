module test(io,data,clk,set,reset,en);
inout io;
input data,clk,set,reset,en;
reg q;
wire d;
assign d=data&io;
always @(posedge clk)
  if(~reset)
     q='b0;
  else if(set)
     q='b1;
  else
     q=d;
assign io= (en?q:'bz);
endmodule

