module test (c); //warning on PORT "c"
output [7:0] c;
test1 t1_i (c);
endmodule
module test1 (c);
output [7:0] c;
wire a;
wire [7:0] c;
not n1 (a, c[1]); // Warning as c is floating
not n2 (c[2], a); // No Warning as c in not floating
endmodule
