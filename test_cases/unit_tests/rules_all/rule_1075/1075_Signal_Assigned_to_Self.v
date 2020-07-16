module test (out, in);
output [3:0] out;
input [3:0] in;
reg [3:0] out;
always @( in or out ) begin
out[0] = out[0];
//warning on "out[0]"
out[1] = out[1] & 1; //warning on "out[1]"
out[2] = out[2] | 'h0; //warning on "out[2]"
out[3] = in[3];
end
endmodule

