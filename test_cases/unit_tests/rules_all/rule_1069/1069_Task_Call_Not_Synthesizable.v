module test;
 reg clock;
 reg a;
 reg b;
 reg y;
 always @(posedge clock)
   multiply(a, b, y); //"multiply" non-synthesizable, warning
 task multiply;
   input a;
   input b;
   output y1;
   begin: serialMult
     reg c, d;
     c = a;
     d = b;
     y1 = 0;
     repeat (5)
       begin
        y1 = c & d;
       end
   end
 endtask
endmodule

