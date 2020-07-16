module test (clock1, clock2, reset, count, data, y);
 input clock1,clock2,reset,data;
 output [8:0] count;
 reg [8:0] count;
 output y;
 reg y;
// initial
//   count <= 0;
 always @(posedge clock1 or negedge reset)
   begin
   if (~reset) //low active "reset", warning on "reset"
     count = 0;
   else
     count = count + 1;
   end
 always @(posedge clock2)
   if (reset) //high active "reset"
     y = 0;
   else
     y = data;
endmodule

