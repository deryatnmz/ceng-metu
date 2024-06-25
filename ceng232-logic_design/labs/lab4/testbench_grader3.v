`timescale 1ns / 1ps
// TO USE THIS TESTBENCH on XILINX:
// Select "Simulation -> Run All" from menu
//Check red light duration changes with respect to the number of cars on the roads
module testbench_grader3(
    );
	 
	wire        greenForA, greenForB, rushHourWarning, am_pm;
	wire [3:0]  hour;
	wire [5:0]  minute;	
	wire [5:0]  second;
	reg  [4:0]  plateIn;
	wire [4:0]  numOfCarsA,numOfCarsB,blackListDisplay;
	wire [6:0]  remainingTime;
	reg  [2:0]  mode; //1xx:display, 0: remA, 1:remB, 2:addA, 3:addB
	reg action;
	reg clk;
	
	//locals
	reg [6:0] remainingTimeAprev, remainingTimeBprev;
	integer i,k;
	integer result;
	reg [100:0] str;
	//end locals

	// Instantiate the Unit Under Test (UUT)
   IntersectionSimulator uut(mode, plateIn, action, clk, greenForA, greenForB, rushHourWarning, hour, minute, second, am_pm, numOfCarsA, numOfCarsB, remainingTime, blackListDisplay);

	// At 5, 15, 25, .. clk will change 0->1
	// At 10, 20, 30, .. clk will change 1->0
   initial clk = 0;
	always #5 clk = ~clk;
	
	task switchTheLight;
	begin
		k=0;
		if (greenForA)
			while(greenForA && k<1000)
			begin
				k=k+1;
				#1;
			end //while
		else
			while(greenForB && k<1000)
			begin
				k=k+1;
				#1;
			end //while	
		#10; //red red condition
	end
	endtask
	
	initial begin
	result=0;
	
	//CASE 1: Check initial condition, greenForA=1, greenForB=0 , remainingTime for B=50 (now roadA=Green, roadB=Red)
	//(roadA=Green, roadB=Red)
	mode=3; plateIn=0;action=0; 
	#1;
	str="CASE 1_";
	if (remainingTime==50 && greenForA==1 && greenForB==0 && numOfCarsA==0 &&numOfCarsB==0) 
	begin
		result = result + 1; 
		$display("%t: %s%d passed.", $time, str,3'b001);
	end else $display("%t: %s%d failed!!", $time, str,3'b001);
	
	//ADD 11 cars to roadB for case 3 
	mode=3; //addB
	for(i = 1; i <= 11; i=i+1) begin
		action=0;
		#1;
		plateIn=i;
		action=1;
		#1;
	end
	//(plates on roadB: 1	2	3	4	5	6	7	8	9	10	11)
	
	//check if lights are RED,RED when remaining time is 0
	i=0;
	while (remainingTime>0 && i<1000) begin
		#1;
		i=i+1;
	end
	if (greenForA==0 && greenForB==0) 
	begin
		result = result + 1; 
		$display("%t: %s%d passed.", $time, str,3'b010);
	end else $display("%t: %s%d failed!!", $time, str,3'b010);




	//CASE 2: Check greenForA=0, greenForB=1 , remainingTime for A=40 
	str="CASE 2";
	switchTheLight; // Switch the lights (now roadA=Red, roadB=Green)
	if (remainingTime==40 && greenForA==0 && greenForB==1 && numOfCarsA==0 &&numOfCarsB==11) 
	begin
		result = result + 1; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);
	
	//ADD 11 cars to roadA for case 4 
	mode=2; //addA
	for(i = 1; i <= 11; i=i+1) begin
		action=0;
		#1;
		plateIn=i+11;
		action=1;
		#1;
	end
	//(plates on road A: 12,13..20,21,22)
	//remove 5 cars from roadB for case 5 
	mode=1; //remB 
	for(i = 1; i <= 5; i=i+1) begin
		action=0;
		#1;
		action=1;
		#1;
	end
	//(plates on road B: 6	7	8	9	10	11)
		
	//CASE 3:  Check greenForA=1, greenForB=0 , remainingTime for B=50 ,numOfCarsA=11, numOfCarsB==6
	str="CASE 3";
	switchTheLight; // Switch the lights (now roadA=Green, roadB=Red)
	if (remainingTime==50 && greenForA==1 && greenForB==0 && numOfCarsA==11 &&numOfCarsB==6) 
	begin
		result = result + 2; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);
	
	//remove 6 cars from roadA for case 5
	mode=0; //remA 
	for(i = 1; i <= 6; i=i+1) begin
		action=0;
		#1;
		action=1;
		#1;
	end
	//(plates on road A:18	19	20	21	22)
	//CASE 4: 
	str="CASE 4";
	switchTheLight; // Switch the lights (now roadA=Red, roadB=Green)
	if (remainingTime==40 && greenForA==0 && greenForB==1 && numOfCarsA==5 &&numOfCarsB==6) 
	begin
		result = result + 2; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);	
	
	//CASE 5: Check decreasing increasing times in regular hours 
	str="CASE 5_";
	remainingTimeAprev=40;
	remainingTimeBprev=50;
	for (i=1;i<=12;i=i+1)
	begin
		switchTheLight; 
		if (greenForB)
		begin	
			if (remainingTime==remainingTimeAprev+5) 
			begin
				result = result + 1; 
				$display("%t: %s%d passed.", $time, str,i[3:0]);
				remainingTimeAprev = remainingTimeAprev + 5;
			end else $display("%t: %s%d failed!!", $time, str,i[3:0]);	
		end else if (greenForA)
		begin	
			if (remainingTime==remainingTimeBprev+5) 
			begin
				result = result + 1; 
				$display("%t: %s%d passed.", $time, str,i[3:0]);
				remainingTimeBprev=remainingTimeBprev + 5;
			end else $display("%t: %s%d failed!!", $time, str,i[3:0]);	
		end
	end //for
	//CASE 6: check if red light durations on roadA and roadB stays at maximum at regular hours
	str="CASE 6_";
	switchTheLight; // Switch the lights (now roadA=Green, roadB=Red)
	if (remainingTime==80) 
	begin
		result = result + 1; 
		$display("%t: %s%d passed.", $time, str,3'b001);
	end else $display("%t: %s%d failed!!", $time, str,3'b001);
	switchTheLight; // Switch the lights (now roadA=Red, roadB=Green)
	if (remainingTime==70) 
	begin
		result = result + 1; 
		$display("%t: %s%d passed.", $time, str,3'b010);
	end else $display("%t: %s%d failed!!", $time, str,3'b010);
	
	//CASE 7: Check rush hour maximums.
	str="CASE 7_";
	i=0;
	while (!rushHourWarning && i<40)
	begin
		switchTheLight;
		i=i+1;
	end
	
	for (i=1;i<=2;i=i+1)
	begin
		switchTheLight; 
		if (greenForB)
		begin	
			if (remainingTime==60) 
			begin
				result = result + 1; 
				$display("%t: %s%d passed.", $time, str,i[3:0]);
				remainingTimeAprev = remainingTimeAprev + 5;
			end else $display("%t: %s%d failed!!", $time, str,i[3:0]);	
		end else if (greenForA)
		begin	
			if (remainingTime==70) 
			begin
				result = result + 1; 
				$display("%t: %s%d passed.", $time, str,i[3:0]);
				remainingTimeBprev=remainingTimeBprev + 5;
			end else $display("%t: %s%d failed!!", $time, str,i[3:0]);	
		end
	end //for	
	//CASE 8: Check decreasing remaining times on road B at rush hour.
	str="CASE 8_";
	//remove all the cars from roadA
	mode=0; //remA 
	for (i=1;i<=5;i=i+1)
	begin
		action=0;
		#1;
		action=1;
		#1;
	end
	
	//remove all the cars from roadB
	mode=1; //remB
	for (i=1;i<=6;i=i+1)
	begin
		action=0;
		#1;
		action=1;
		#1;
	end
		
	if (greenForB)	switchTheLight; //to make sure B is red
	
	//ADD 20 cars to roadB  
	mode=3; //addB
	for(i = 1; i <= 20; i=i+1) begin
		action=0;
		#1;
		plateIn=i;
		action=1;
		#1;
	end
	
	remainingTimeBprev=70;
	for (i=1;i<=6;i=i+1)
	begin
		switchTheLight; 
		switchTheLight; //(now roadA=Green, roadB=Red)			
		if (remainingTime==remainingTimeBprev-5) 
		begin
			result = result + 1; 
			$display("%t: %s%d passed.", $time, str,i[3:0]);
			remainingTimeBprev=remainingTime;
		end else $display("%t: %s%d failed!!", $time, str,i[3:0]);	
	end //for
	
	//CASE 9: Check if red light duration on roadB stays at minimum
	str="CASE 9";
	switchTheLight;
	switchTheLight; // (now roadA=Green, roadB=Red)
	if (remainingTime==40) 
	begin
		result = result + 1; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);
	
	//CASE 10: Check if red light duration on roadB reached to max. at rush hour
	str="CASE 10";
	switchTheLight; // (now roadA=Red, roadB=Green)
	//remove all the cars from roadB
	mode=1; //remB 
	for (i=1;i<=20;i=i+1)
	begin
		action=0;
		#1;
		action=1;
		#1;
	end
	switchTheLight; //(now roadA=Green, roadB=Red) //remaining time for B = 45
	for (i=1;i<=6;i=i+1)
	begin
		switchTheLight;
		switchTheLight; //(now roadA=Green, roadB=Red)
	end
	if (remainingTime==70) 
	begin
		result = result + 1; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);
		 
	
	
	
	$display("Result is:%d",result); //max 31
	$finish;

	end
      
endmodule

