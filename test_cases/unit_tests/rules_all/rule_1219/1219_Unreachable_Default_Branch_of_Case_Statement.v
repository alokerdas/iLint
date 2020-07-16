module test(foo, bar, bar_0, bar_1, bar_2, bar_3);
input [5:0] foo;
input [35:0] bar_0, bar_1, bar_2, bar_3;
output [35:0] bar;
reg [35:0] bar;
always @( foo or bar_0 or bar_1 or bar_2 or bar_3 ) begin
  case (foo[1:0])
   2'd0 : bar = bar_0;
   2'd1 : bar = bar_1;
   2'd2 : bar = bar_2;
   2'd3 : bar = bar_3;
   default : bar = 36'd0; //warning here
  endcase
end
endmodule

