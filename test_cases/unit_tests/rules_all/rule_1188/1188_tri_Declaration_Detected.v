module test(a,b,t);
input a,b;
output t;
tri t;
bufif0 B(t,a,b);
endmodule

