module top (o, a, b);
 input a, b;
 output o;
 test test(o, a, b); //instance name should not be
                //the same as module name,
                //"u_test_1" is recommended
endmodule
module test (o, a, b);
 input a, b;
 output o;
 and and1 (o,a,b);
endmodule

