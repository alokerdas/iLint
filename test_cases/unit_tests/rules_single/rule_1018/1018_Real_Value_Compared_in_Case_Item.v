`define AA 1.1
module test;
 reg a,b,d;
 real c;
 parameter k=1.0;
 initial begin
   a=`AA;
   c=1.2;
   case (a)
    a             : b=0;
    1             : b=0;
    `AA             : b=1; // Warning Here
    k             : b=1; // Warning Here
    1.1,c           : b=1; // Warning Here
    d             : b=1;
    default          : b=0;
   endcase
   case (b)
    `AA+1             : a=1; // Warning Here
    k+1.0            : a=1; // Warning Here
    c+1             : a=1; // Warning Here
    `AA+((c+1)+(k+0.11)) : a=1; // Warning Here
    k+`AA+((c+1)+(k+0.11)) : a=1; // Warning Here
    default          : a=0;
   endcase
 end
endmodule

