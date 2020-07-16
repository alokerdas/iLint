module test (clk,rst_p,q);
 inout clk,rst_p;
 output q;
 reg q;
 reg q_nxt;
 always @(posedge clk)
   begin: Dff //a meaningful name is attached on process
    if ( rst_p )
       q = 0;
    else
       q = q_nxt;
   end
endmodule

