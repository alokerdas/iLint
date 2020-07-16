module test( data, sel, out );
input [2:0] data;
input [1:0] sel;
output out;
reg out;
always @( data or sel ) begin
case ( sel ) //synopsys full_case
2'b10 : out = data[0];
2'b11 : out = data[1];
2'b01 : out = data[2];
endcase
end
endmodule


