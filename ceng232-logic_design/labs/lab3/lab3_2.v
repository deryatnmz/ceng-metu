`timescale 1ns / 1ps
module QueueCounter(
	input [7:0] llmt,
	input [7:0] ulmt,
	input en,
	input mode, // 1:delete, 0:insert
	input clk,
	output reg [7:0] head,
	output reg [7:0] tail,
	output reg empty,
	output reg full);
	
	//you may write your code here

	initial
	begin
		head = llmt;
		tail = llmt;
	end

	//you may write your code here
 initial
  begin
              empty = 1'b1;
      	full= 1'b0;
  end

	always@(posedge clk )
		if(en == 1'b1) 
		begin

			if(mode==1'b0)
			begin
				if(full!=1'b1)
				begin
					if((tail + 1'b1) > ulmt)
						begin
							tail = llmt;
						end
					else
					begin
						tail = tail + 1'b1;
					end
					empty=1'b0;
					if(tail-head==1'b0)
					begin
						full=1'b1;
					end	
				end
			end

			else if (mode==1'b1)
			begin
				if(empty!=1'b1)
				begin
					if((head + 1'b1) > ulmt)
					begin
						head = llmt;
					end
					else
					begin
						head = head + 1'b1;
					end

					if(tail-head==1'b0)
					begin
						empty=1'b1;
					end
                  	full=1'b0;

				end
			end
		end

endmodule


module Numerator(
	input clinic, 
	input mode, 
	input en, 
	input clk,
	output [7:0] ann,
	output [7:0] print);

	//write your code here
		wire [7:0] lowe;
    reg ann;
    reg print;
    wire [7:0] ann1;
    wire [7:0] ann2;
    wire [7:0] print1;
    wire [7:0] print2;
    

	
     QueueCounter q1(8'd5,8'd9,en& ~clinic,mode,clk,ann1,print1,,);
     QueueCounter q2(8'd10,8'd14,en& clinic,mode,clk,ann2,print2,,);
    always@(*)
    
	begin
		if(clinic == 1'b0) 
		begin
	       ann=ann1;
	       print=print1;
	    end
	    else if (clinic == 1'b1) 
	    begin
           ann=ann2;
	       print=print2;
	    end
	
	end

endmodule
