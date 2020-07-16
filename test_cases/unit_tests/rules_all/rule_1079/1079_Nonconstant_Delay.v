module test (clock, q_nxt, q);
 input clock, q_nxt;
 output q;
 reg q;
 reg [2:0] a;
 initial
   a = 2;
 always
   #4 a = ~a;
 always @(posedge clock)
   q = #a q_nxt; //warning on "a",
            //delay value is not static constant
endmodule

