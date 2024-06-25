`timescale 1ns / 1ps
module kb(
	input K,
	input B,
	input clk,
	output reg Q
	);
    
	initial begin
		Q = 0;
	end
	 
	// write your code here
	always @ (posedge clk)  
      case ({K,B})  
         2'b00 :  Q <= ~Q;  
         2'b01 :  Q <= 0;  
         2'b10 :  Q <= 1;  
         2'b11 :  Q <= Q;  
    endcase  

endmodule


module ic232(
	input A0, 
	input A1, 
	input A2, 
	input clk,
	output Q0,
	output Q1,
	output Z
	);
   
	// write your code here
	wire W1,W2,W3,W4;
    nor(W1,A0,A1);
    and(W2,W1,~A2);
    kb kb_1(W2,A1,clk,Q0);
    or(W3,~A0,A1);
    xor(W4,W3,A2);
    kb kb_2(~A2,W4,clk,Q1);
    xnor(Z,Q0,Q1);
	
endmodule