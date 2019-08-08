////////////////////////////////////////////////////////////////////////////////
// File				: fsm2hdl.v
// Author			: Amir Mohammad Biuki (DarkC343)
// First release	: August 6, 2019
// Date				: Fri Aug 9 2019
// Source link		: https://github.com/DarkC343/fsm2hdl
// University		: Shahed (shahed.ac.ir)
// Email			: dark.c343@gmail.com
// This verilog module is a auto-generated Mealy Finite State Machine using
// 'fsm2hdl' tool. This tool is currently under development using Qt/C++.
// Read README.me for installation. New features will be added soon!
// ------------------------------------------------ 
//	FSM type: Mealy
//	Number of states: 5
//	Clock edge: rising edge
//	Reset edge: falling edge
//	Reset mode: asynchronous
//	Fail safe state name: S0
//	Fail safe default output: 0
// ------------------------------------------------ 
//	* State: S0 is connected to S1 giving input 10. Output becomes 1
//	* State: S2 is connected to S1 giving input 0x. Output becomes 0
//	* State: S3 is connected to S3 giving input x0. Output becomes 1
//	* State: S4 is connected to S4 giving input 11. Output becomes 1
//	* State: S0 is connected to S0 giving input 00. Output becomes 0
//	* State: S0 is connected to S2 giving input 11. Output becomes 1
//	* State: S1 is connected to S1 giving input x1. Output becomes 0
//	* State: S2 is connected to S3 giving input 1x. Output becomes 1
//	* State: S3 is connected to S4 giving input x1. Output becomes 1
//	* State: S1 is connected to S0 giving input 00. Output becomes 0
//	* State: S1 is connected to S2 giving input 10. Output becomes 0
//	* State: S0 is connected to S4 giving input 01. Output becomes 1
//	* State: S4 is connected to S0 giving input 0x. Output becomes 0
//	* State: S4 is connected to S0 giving input 10. Output becomes 1
////////////////////////////////////////////////////////////////////////////////

`timescale 1ns / 1ps

module fsm2hdl_mealy(rst, clk, in, out);
	input [1:0] in;
	input rst, clk;
	output reg out;
	
	reg [2:0] curr_s, next_s;
	parameter S0 = 3'b000;
	parameter S1 = 3'b001;
	parameter S2 = 3'b010;
	parameter S3 = 3'b011;
	parameter S4 = 3'b100;

	always @(posedge clk or negedge rst)
		if(rst == 1'b0)
			curr_s <= S0;
		else
			curr_s <= next_s;
	
	always @(in or curr_s)
		case(curr_s)
			S0:
				case(in)
					2'b10: next_s = S1;
					2'b00: next_s = S0;
					2'b11: next_s = S2;
					2'b01: next_s = S4;
				endcase
			S1:
				case(in)
					2'b11, 2'b01: next_s = S1;
					2'b00: next_s = S0;
					2'b10: next_s = S2;
				endcase
			S2:
				case(in)
					2'b01, 2'b00: next_s = S1;
					2'b11, 2'b10: next_s = S3;
				endcase
			S3:
				case(in)
					2'b10, 2'b00: next_s = S3;
					2'b11, 2'b01: next_s = S4;
				endcase
			S4:
				case(in)
					2'b11: next_s = S4;
					2'b01, 2'b00: next_s = S0;
					2'b10: next_s = S0;
				endcase
			default: next_s = S0;
		endcase

	always @(in or curr_s)
		case(curr_s)
			S0:
				case(in)
					2'b10: out = 1'b1;
					2'b00: out = 1'b0;
					2'b11: out = 1'b1;
					2'b01: out = 1'b1;
				endcase
			S1: out = 1'b0;
			S2:
				case(in)
					2'b01, 2'b00: out = 1'b0;
					2'b11, 2'b10: out = 1'b1;
				endcase
			S3: out = 1'b1;
			S4:
				case(in)
					2'b11: out = 1'b1;
					2'b01, 2'b00: out = 1'b0;
					2'b10: out = 1'b1;
				endcase
			default: out = 1'b0;
		endcase
endmodule

