module test2 (sel1, sel2, a, b, out);
input sel1, sel2, a, b;
output out;
assign out = sel1? a: 1'bz;
assign out = sel2? b: 1'bz;
endmodule

