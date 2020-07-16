module test();
wire carryOut, carryIn, aIn, bIn;
Carry u1( carryOut, carryIn, aIn, bIn );
endmodule
primitive Carry(carryOut,carryIn,aIn,bIn);//Warning here
output carryOut;
input carryIn, aIn,bIn;
table
// carryIn aIn bIn carryOut
0 0 0 : 0;
0 0 1 : 0;
0 1 0 : 0;
0 1 1 : 1;
1 0 0 : 0;
1 0 1 : 1;
1 1 0 : 1;
1 1 1 : 1;
endtable
endprimitive

