////////////////////////////////////////////////////////////////////////////////
// File				: fsm2hdl.v
// Author			: Amir Mohammad Biuki (DarkC343)
// First release	: August 6, 2019
// Date				: Fri Aug 9 2019
// Source link		: https://github.com/DarkC343/fsm2hdl
// University		: Shahed (shahed.ac.ir)
// Email			: dark.c343@gmail.com
// This verilog module is a auto-generated Moore Finite State Machine using
// 'fsm2hdl' tool. This tool is currently under development using Qt/C++.
// Read README.me for installation. New features will be added soon!
// ------------------------------------------------ 
//	FSM type: Moore
//	Number of states: 5
//	Clock edge: rising edge
//	Reset edge: falling edge
//	Reset mode: asynchronous
//	Fail safe state: S0
// ------------------------------------------------ 
//	* State: [S0] is connected to [S1] giving input 1. Output 0 changes to 0 respectively
//	* State: [S0] is connected to [S0] giving input 0. Output 0 changes to 0 respectively
//	* State: [S1] is connected to [S0] giving input 0. Output 0 changes to 0 respectively
//	* State: [S11] is connected to [S11] giving input 1. Output 0 changes to 0 respectively
//	* State: [S1101] is connected to [S11] giving input 1. Output 1 changes to 0 respectively
//	* State: [S110] is connected to [S1101] giving input 1. Output 0 changes to 1 respectively
//	* State: [S1101] is connected to [S0] giving input 0. Output 1 changes to 0 respectively
//	* State: [S11] is connected to [S110] giving input 0. Output 0 changes to 0 respectively
//	* State: [S110] is connected to [S0] giving input 0. Output 0 changes to 0 respectively
//	* State: [S1] is connected to [S11] giving input 1. Output 0 changes to 0 respectively
////////////////////////////////////////////////////////////////////////////////

`timescale 1ns / 1ps

module fsm2hdl_moore(rst, clk, in, out);
	input in;
	input rst, clk;
	output reg out;
	
	reg [2:0] curr_s, next_s;
	parameter S0 = 3'b000;
	parameter S1 = 3'b001;
	parameter S11 = 3'b010;
	parameter S110 = 3'b011;
	parameter S1101 = 3'b100;

	always @(posedge clk or negedge rst)
		if(rst == 1'b0)
			curr_s <= S0;
		else
			curr_s <= next_s;
	
	always @(in or curr_s)
		case(curr_s)
			S0:
				case(in)
					1'b1: next_s = S1;
					1'b0: next_s = S0;
				endcase
			S1:
				case(in)
					1'b0: next_s = S0;
					1'b1: next_s = S11;
				endcase
			S11:
				case(in)
					1'b1: next_s = S11;
					1'b0: next_s = S110;
				endcase
			S110:
				case(in)
					1'b1: next_s = S1101;
					1'b0: next_s = S0;
				endcase
			S1101:
				case(in)
					1'b1: next_s = S11;
					1'b0: next_s = S0;
				endcase
			default: next_s = S0;
		endcase

	always @(curr_s)
		case(curr_s)
			S0: out = 1'b0;
			S1: out = 1'b0;
			S11: out = 1'b0;
			S110: out = 1'b0;
			S1101: out = 1'b1;
			default: out = 1'b0;
		endcase
endmodule

