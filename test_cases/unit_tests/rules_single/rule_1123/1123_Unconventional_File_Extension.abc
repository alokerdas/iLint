module test (sel,a,b,s);
  input sel, a, b;
  output s;
  reg s;
  always begin
    if ( sel == 0 ) begin
      s <= a;
      @( sel or a );
    end
    else //warning here, there is no event control
          //statement in this branch
      s <= b;
  end
endmodule

