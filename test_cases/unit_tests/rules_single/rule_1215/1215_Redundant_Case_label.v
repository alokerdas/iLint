module test (y,clock,reset,control);
 input clock,reset,control;
 output [2:0] y;
 reg [2:0] y;
 wire clock,reset,control;
 parameter ST0 = 0,ST1 = 1,ST2 = 2,ST3 = 3;
 reg[1:0] current, next;
 always @(control or current)
   begin
    case (current)
    ST0,ST0: begin y <= 1; next <= ST1; end
     //warning here, redundant case expression
    ST1: begin
         y <= 2;
         if (control) next <= ST2;
         else next = ST3;
       end
    ST2: begin y <= 3; next <= ST3; end
    ST3: begin y <= 4; next <= ST0; end
    default: begin y <= 1; next <= ST0; end
    endcase
 end
endmodule
