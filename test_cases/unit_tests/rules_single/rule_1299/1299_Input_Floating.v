module test1 (o, a);
input a;
output o;
wire b;
wire [7:0] c;
not n1 (a, b); // Warning as c is floating
not n2 (o, a); // No Warning as c in not floating
endmodule
