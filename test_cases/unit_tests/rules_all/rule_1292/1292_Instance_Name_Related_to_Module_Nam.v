module adderx(ss);
 input [1:0] ss;
 parameter check = 1;
endmodule
module adderx1(ss);
 input [1:0] ss;
 parameter check = 1;
endmodule
module test_adder;
 wire [1:0] in1;
 adderx adderx_1(in1);       // OK
 adderx adderx_2(in1);       // OK
 adderx test_1_adderx(in1);     // OK
 adderx testadder(in1);     // Waring,instname does not relate to module name.
                                    // Warning,instname does not have an index.
 adderx adderx_last(in1);     // Warning,instname does not have an index.
 adderx T111111_eSt_tEsT_11_(in1); // Waring,instname does not relate to module name.
 adderx adder_last_1(in1);     // Waring,instname does not relate to module name.
 adderx a_0001A_1 (in1);        // Waring,instname does not relate to module name.
 adderx1 adderx1a_1(in1);        // OK
 adderx1 adDabcerxd_0(in1);         // Waring,instname does not relate to module name.
 adderx1 AdDabcerxd2(in1);         // Waring,instname does not relate to module name.
                              // Warning,instname does not have an index.
 adderx1 addabcerxe1(in1);        // Waring,instname does not relate to module name.
                              // Warning,instname does not have an index.
endmodule

