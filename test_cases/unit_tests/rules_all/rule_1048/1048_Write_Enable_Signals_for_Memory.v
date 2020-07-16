module test(test_mode, WE, WE_mem);
 input test_mode;
 input WE;
 output WE_mem;
 assign WE_mem = test_mode | WE;
 //code inferred to memory operation
 //...
endmodule

