module test (sel,a,b,c,y);
  input sel;
  input [7:0] a,b,c;
  output [7:0] y;
  reg [7:0] y; //"y" in scope "block1"
  reg [7:0] temp;
  always @( sel or a or b or c)
    begin: testb
      reg [7:0] y; //"y" in scope "testb",
                   //duplicate with up-scope, warning
      temp = 0;
      if ( sel )
        temp = b;
      else
        temp = c;
      y = a+temp;
    end
endmodule

