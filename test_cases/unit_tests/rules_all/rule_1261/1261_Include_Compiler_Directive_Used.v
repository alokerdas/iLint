`include "test.i"
//"`include" directive unrecommended, warning
module inival;
reg a,b;
wire c;
initial
begin
$monitor($time,,,"a=%d, b=%d, c=%d",a,b,c);
a=1;
b=0;
#10 a = 0;
#10 a = 1;
#10 b = 1;
end
endmodule

