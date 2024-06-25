// Code your design here
`timescale 1ns / 1ps
module IntersectionSimulator(
	input [2:0] mode, //1xx:display, 000:remA, 001:remB, 010:addA, 011:addB
  input [4:0] plateIn,
	input action,
	input clk,
	output reg  greenForA,
	output reg  greenForB,
	output reg 	rushHourWarning,
	output reg [3:0]  hour,
	output reg [5:0]  minute,	
	output reg [5:0]  second,
	output reg	am_pm, //0:am, 1:pm
	output reg [4:0]  numOfCarsA,
	output reg [4:0]  numOfCarsB,
	output reg [6:0]  remainingTime,
	output reg [4:0]  blackListDisplay
	);
	
	//You may write your code anywhere
  reg [4:0]  road_A [3:0]; 
  reg [4:0]  road_B [3:0]; 
  reg [4:0]  blackList [3:0]; 
  integer road_A_beginning;
  integer road_B_beginning;
  integer black_list_end;
  integer numOfBlackList; 
  integer i=0;
  integer j=0;
  integer swic=0;
  integer prevA=1;
  integer nextDurA=40;
  integer nextDurB=50;  
  
	initial begin
		greenForA=1;
		greenForB=0;
		rushHourWarning=0;
		hour=6;
		minute=0;
		second=0;
		am_pm=0;
		numOfCarsA=0;
		numOfCarsB=0;
		remainingTime=50;
		blackListDisplay=0;
		//...
        for(i=0;i<30;i=i+1) 
			begin
              road_A[i]=5'd0;
              road_B[i]=5'd0;
              blackList[i]=5'd0;
			end
        road_A_beginning = 0;
        road_B_beginning = 0;
	    black_list_end = 0;
        numOfBlackList = 0;
		
	end
  

	always@(posedge action)
	begin
      
      if(mode == 3'b000)
        begin
          if (numOfCarsA > 0)
            begin
              if(greenForA==0)
        	    begin
                  blackList[black_list_end]=road_A[road_A_beginning];
             	 black_list_end=black_list_end + 1;
            	end
              
              
                numOfCarsA = numOfCarsA - 1;
              if (road_A_beginning==29)
                road_A_beginning = 0;
              else
                road_A_beginning=road_A_beginning+1;
              
            end    
        end
      
      if(mode == 3'b001)
        begin
          if (numOfCarsB > 0)
            begin
              
              if(greenForB==0)
              begin
                								
              blackList[black_list_end]=road_B[road_B_beginning];
                black_list_end=black_list_end + 1;
              end
              
                numOfCarsB = numOfCarsB - 1;
              if (road_B_beginning==29)
                road_B_beginning = 0;
              else
                road_B_beginning=road_B_beginning+1;
              
            end
        end
      
      if(mode == 3'b010)
        begin
          if(road_A_beginning + numOfCarsA < 30 )
            road_A[road_A_beginning + numOfCarsA]=plateIn;
          else
            begin
            road_A[(road_A_beginning + numOfCarsA)-30]=plateIn;
            end
          
		  numOfCarsA = numOfCarsA + 1;
        end
      
      if(mode == 3'b011)
        begin
          if(road_B_beginning + numOfCarsB < 30 )
            road_B[road_B_beginning + numOfCarsB]=plateIn;
          else
            begin
              road_B[(road_B_beginning + numOfCarsB)-30]=plateIn;
            end
          
		  numOfCarsB = numOfCarsB + 1;
          
        end
      
      
	end

  always@(*)
		begin
          if(hour==12 & minute ==0 & second == 0 & am_pm==0) 
        begin

            blackListDisplay=0;
          black_list_end=0;
          j=0;
          for(i=0;i<30;i=i+1) 
			begin
              blackList[i]=5'd0;
			end
        end
        end
      
      

	always @(posedge clk)
	begin      
      if(mode != 3'b1xx)
        begin
          if(second==6'd59)
          begin
          second=0;
            if(minute ==6'd59)
              begin
                minute = 0;
                if (hour==4'd11)
                  begin
                    hour=4'd12;
                    am_pm = ~am_pm;
                  end
                else if(hour == 4'd12)
                  begin
                    hour=1;
                  end
                else
                  hour = hour+1;
              end
            else
              minute = minute+1;
          end
        else
          begin
          second = second + 1;
          end
       		remainingTime=remainingTime-1;
        end
      
      else
    	    begin
              blackListDisplay=blackList[j];
              if(j+1==black_list_end)
                  	j=0;
      
                else if(j+1<black_list_end)
                    j=j+1;
       		 end
      if((4'd9 > hour & hour>= 4'd7 & am_pm==0)|(4'd7 > hour & hour>= 4'd5 & am_pm==1))
        begin
      rushHourWarning=1;
        end
      else
      rushHourWarning=0;
      
      if(swic==1)
        begin 
          if(prevA==1)
            begin
            remainingTime=nextDurA;
            greenForB=1;
            greenForA=0;
            prevA=0;
            end
          else if(prevA==0)
            begin
            remainingTime=nextDurB;
            greenForB=0;
            greenForA=1;
            prevA=1;              
            end
           swic=0;  
        end
      
      if(remainingTime==0)
        begin
          swic=1;
          greenForA=0;
          greenForB=0;
          
          if(rushHourWarning==0)
            begin
              
              if(prevA==1)
                begin
                  if(numOfCarsB>=0 & 10>=numOfCarsB)
                    begin
                      nextDurB=nextDurB+5;
                    end
                  else if(numOfCarsB>=20)
                    begin
                      nextDurB=nextDurB+5;
                    end
                  if(nextDurB<50)
                    nextDurB=50;
                  if(nextDurB>80)
                    nextDurB=80;
                end
              else if(prevA==0)
                begin
                  if(numOfCarsA>=0 & 10>=numOfCarsA)
                    begin
                      nextDurA=nextDurA+5;
                    end
                  else if(numOfCarsA>=20)
                    begin
                      nextDurA=nextDurA-5;
                    end 
                  if(nextDurA<40)
                    nextDurA=40;
                  if(nextDurA>70)
                    nextDurA=70;
                end
              
            end
          else if (rushHourWarning==1)
            begin
              if(prevA==1)
                begin
                  if(numOfCarsB>=0 & 10>=numOfCarsB)
                    begin
                      nextDurB=nextDurB+5;
                    end
                  else if(numOfCarsB>=20)
                    begin
                      nextDurB=nextDurB-5;
                    end
                  if(nextDurB<40)
                    nextDurB=40;
                  if(nextDurB>70)
                    nextDurB=70;
                end
              else if(prevA==0)
                begin
                  if(numOfCarsA>=0 & 10>=numOfCarsA)
                    begin
                      nextDurA=nextDurA+5;
                    end
                  else if(numOfCarsA>=20)
                    begin
                      nextDurA=nextDurA-5;
                    end 
                  if(nextDurA<30)
                    nextDurA=30;
                  if(nextDurA>60)
                    nextDurA=60;
                end           
            end
          
          
        end



      
	end
  
  


endmodule



