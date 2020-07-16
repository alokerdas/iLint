module adder0(.a(in1),.b(in2),.ci(carry_in),.sum(sum),.co(carry_out));
output sum,carry_out;
input in1,in2,carry_in;
reg sum,carry_out;
always @(in1 or in2 or carry_in)
{carry_out,sum} = in1+in2+carry_in;
endmodule

