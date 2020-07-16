module test (Clock);
 input Clock;
 reg state;
 reg out;
 always@(Clock)
   begin
    casez (state)
     1'bz: out = 1;
     1'bx: out = 0; //warning on "1'bx"
     default: out = 1;
    endcase
   end
endmodule

