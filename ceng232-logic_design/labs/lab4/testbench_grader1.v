`timescale 1ns / 1ps
// TO USE THIS TESTBENCH on XILINX:
// Select "Simulation -> Run All" from menu
//Check rushHourWarning, am_pm


module testbench_grader1(
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
	integer i;
	integer result;
	reg [100:0] str;
	//end locals

	// Instantiate the Unit Under Test (UUT)
   IntersectionSimulator uut(mode, plateIn, action, clk, greenForA, greenForB, rushHourWarning, hour, minute, second, am_pm, numOfCarsA, numOfCarsB, remainingTime, blackListDisplay);

	// At 5, 15, 25, .. clk will change 0->1
	// At 10, 20, 30, .. clk will change 1->0
   initial clk = 0;
	always #5 clk = ~clk;
	
	
	initial begin
	result=0;
	
	//CASE 1: Check initial condition
	mode=3; plateIn=0;action=0;
	#1; 
	str="CASE 1";
	if (hour==6 && minute==0 && second==0 && am_pm==0) 
	begin
		result = result + 1; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);
	


	//CASE 2: Check if rush hour warning is 1 at 07:00:00 AM
	str="CASE 2";
	i=0;
	while (~rushHourWarning & i<40000)
	begin
		#1;
		i=i+1;
	end
	if (rushHourWarning ==1'b1 && hour==7 && minute==0 && second==0 && am_pm==0)
	begin
		result = result + 2; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);
	
	//CASE 3: Check if rush hour warning is 0 at 09:00:00 AM
	str="CASE 3";
	i=0;
	while (rushHourWarning && i<73000)
	begin
		#1;
		i=i+1;
	end
	
	if (rushHourWarning ==1'b0 && hour==9 && minute==0 && second==0 && am_pm==0)
	begin
		result = result + 2; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);
	
	//CASE 4: Check if am_pm is 1 at 12:00:00 PM
	str="CASE 4";
	i=0;
	while (~am_pm && i<110000)
	begin
		#1;
		i=i+1;
	end
	if (hour==12 && minute==0 && second==0 && am_pm==1)
	begin
		result = result + 1; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);	
	
	//CASE 5: Check if rush hour warning is 1 at 05:00:00 PM
	str="CASE 5";
	i=0;
	while (~rushHourWarning && i<185000)
	begin
		#1;
		i=i+1;
	end
	
	if (rushHourWarning ==1'b1 && hour==5 && minute==0 && second==0 && am_pm==1)
	begin
		result = result + 2; 
		$display("%t: %s passed.", $time, str);
		
	end else $display("%t: %s failed!!", $time, str);	
	
	//CASE 6: Check if rush hour warning is 0 at 07:00:00 PM
	str="CASE 6";
	i=0;
	while (rushHourWarning && i<73000)
	begin
		#1;
		i=i+1;
	end
	if (rushHourWarning ==0 && hour==7 && minute==0 && second==0 && am_pm==1)
	begin
		result = result + 2; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);		

	//CASE 7: Check if am_pm is 0 at 12:00:00 AM
	str="CASE 7";
	i=0;
	while (am_pm && i<185000)
	begin
		#1;
		i=i+1;
	end
	if (hour==12 && minute==0 && second==0 && am_pm==0)
	begin
		result = result + 1; 
		$display("%t: %s passed.", $time, str);
	end else $display("%t: %s failed!!", $time, str);		
	

	
	$display("Result is:%d",result); //max. 11
	$finish;

	end
      
endmodule

