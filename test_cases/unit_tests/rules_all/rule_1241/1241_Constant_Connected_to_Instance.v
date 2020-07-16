/*module test;
  wire ta, tb;
  uu u_uu_1( .a(ta), .b() ); //warning here for r22279
  uu u_uu_2( .a(ta) );       //warning here for r22082
  uu u_uu_3( ta, );          //warning here for r22082
endmodule
module uu (a, b);
  input a;
  output b;
  wire a, b;
  assign b = a;
endmodule*/

module test;
  wire a;
  wire c;
  and and1(c, a, 1); //warning on "1" as port instance;
                     //it can be reduced to c=a;
endmodule


