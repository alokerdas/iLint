module test(a,b,t);
input a,b;
output t;
//tri0 t;
 tri0 t;
bufif0 B(t,a,b);
/*
  tri1 tri1_t;//"tri1" non-synthesizable, warning
  tri0 tri0_t;//"tri0" non-synthesizable, warning
  supply0 supply0_t;//"supply0" non-synthesizable, warning
  supply1 supply1_t;//"supply1" non-synthesizable, warning
  triand triand_t;//"triand" non-synthesizable, warning
  trior trior_t;//"trior" non-synthesizable, warning*/
//trireg trireg_t;//"trireg" non-synthesizable, warning
endmodule

