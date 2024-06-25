`timescale 1ns / 1ps
module ROM (
input [2:0] addr, 
output reg [7:0] dataOut);
  reg [7:0] rom [0:7];
  initial begin
    rom[0] = 8'b00000000;
  rom[1] = 8'b01010101;
  rom[2] = 8'b10101010;
  rom[3] = 8'b00110011;
  rom[4] = 8'b11001100;
  rom[5] = 8'b00001111;
  rom[6] = 8'b11110000;
  rom[7] = 8'b11111111;
  end
  always @(addr)
  begin
  dataOut = rom[addr];
  end
endmodule


module XOR_RAM (
input mode, 
input [2:0] addr, 
input [7:0] dataIn, 
input [7:0] mask, 
input CLK, 
output reg [7:0] dataOut);

reg [7:0] ram [0:2];
integer i;
initial begin
  for (i =0 ;i<=7 ;i = i+1 ) begin
    ram[i] = 8'b00000000;
  end
  dataOut=8'b00000000;
end


  always @ (*)
    	begin
          if(mode == 0  &  CLK == 1)
            ram[addr]=mask ^ dataIn;
          if(mode == 1 & addr>=0 & dataIn>=0)
            dataOut = ram[addr];
    	end
   endmodule


module EncodedMemory (
input mode, 
input [2:0] index, 
input [7:0] number, 
input CLK, 
output [7:0] result);
	
	// DO NOT EDIT THIS MODULE
	
	wire [7:0] mask;
	
	ROM R(index, mask);
	XOR_RAM XR(mode, index, number, mask, CLK, result);

endmodule
