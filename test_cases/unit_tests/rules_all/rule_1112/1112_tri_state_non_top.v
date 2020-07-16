module top(count, clock, reset, load, data);
 output count;
 input data;
 input clock, reset, load;
 wire data;
 wire clock, reset, load;
 wire count;
 up_counter u_up_counter_1(count, clock, reset, load, data);
endmodule
module up_counter(out, clock, reset, load, data);
 output out;
 input data;
 input clock, reset, load;
 wire data;
 wire clock, reset, load;
 wire out;
 reg count;
 bufif0 B(out,load,count);
endmodule

