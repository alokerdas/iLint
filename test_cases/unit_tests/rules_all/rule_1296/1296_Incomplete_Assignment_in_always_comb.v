module test( en, a, b, a1, b1, a2, b2 );
input en;
output a, b, a1, b1, a2, b2;
reg a, b, a1, b1, a2, b2;
always_comb begin //warning on tmp.b and tmp.a here
a = 0;
if (en == 1 )
b = 0;
//tmp.b is not assigned in all branches
else a <= 1; // tmp.a is not assigned in all branches
end
always_comb begin //warning on tmp.b1 here
a1 = 0;
//tmp.a is fully assigned
if (en == 1 )
b1 = 0;
//tmp.b is not assigned in all branches
end
always_comb begin // No warning
a2 = 0;
//tmp.a is fully assigned
if (en == 1 )
b2 = 0;
//tmp.b is fully assigned in all branches
else b2 <= 1;
end
endmodule
