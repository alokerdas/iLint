module test;
 parameter n=8; //no warning, not an integer
 integer a;   //warning here
 function func1;
 input f1;
 integer f2; //warning here
   f2=0;
 endfunction
endmodule

