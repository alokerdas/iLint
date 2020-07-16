module test(cLK,carryout,carryin, ain, BIN);
  output carryout,cLK;
  input carryin, ain, BIN;
  CarrY u1( carryout, carryin, ain, BIN );
  not a (cLK,carryout);
endmodule
primitive CarrY(carryout,carryin,ain,bin); //Warning here
  output carryout;
  input carryin, ain,bin;
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

