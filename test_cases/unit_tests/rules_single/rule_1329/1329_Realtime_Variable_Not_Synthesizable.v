module test(i, o);
input i;
output o;
realtime rt;//non-synthesizable, warning
and a0 (o, rt, i);
endmodule

