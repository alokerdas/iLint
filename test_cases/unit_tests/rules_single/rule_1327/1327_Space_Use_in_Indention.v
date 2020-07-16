module Ao4 (rst_n, clk, y, a, b, c, d, q, r);
  output y, q, r;
  input a, b, c, d;
  input clk, rst_n;
  reg y, tmp1, tmp2, q, r;
  wire p;
  always @ (posedge clk) begin
    tmp1 = a & b;
    if ( tmp1 )
      tmp2 <= c & d;
       y <= tmp1 | tmp2;
  end
  assign p = ~tmp2;
  always @(posedge clk or negedge rst_n)
     begin
     //warning here
       if(!rst_n)
          q <= 1'b0;
       else
          q <= d;
     end
     //warning here
 always @ (posedge clk) //warning here
   begin
     r <= a | b;
   end
endmodule

