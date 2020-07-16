module comb1(input a, b ,c, output reg [1:0] y);
task ort1;
input a;
y[0] = a | b | c;
endtask
always_comb ort1(a); //warning here
endmodule
