module test (y, c, d, a, b);
 input a, b;
 output c, d;
 output [1:0] y;
 reg c, d;
 reg [1:0] y;
 always @(a or b)
   begin
   repeat(3) {c, d} = {a, b};//"repeat" non-synthesizable, warning
   y = repeatfun(a, b);
   end
 function [1:0] repeatfun;
   input a, b;
   repeat(6) repeatfun = {b, a};//"repeat" non-synthesizable, warning
 endfunction
endmodule

