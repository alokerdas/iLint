module fsm(out, in, clock, reset);
output out;
input in, clock, reset;
reg out;
reg [1:0] current, next;
always @(in or current) begin
  out = ~current[1] & current[0];
  next = 0;
  if (current == 0)
    if (in)
       next = 1;
  if (current == 1)
    if (in)
       next = 3;
  if (current == 3)
    if (in)
       next = 3;
  else
    next = 1;
end
always @(posedge clock or negedge reset) begin
  if (~reset)
    current <= 0;
  else
    current <= next; //warning here; good style if using "_cs"
                 //as suffix of current state register "current"
  end
endmodule

