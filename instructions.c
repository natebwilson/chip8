#include "global.h"

/** INSTRUCTIONS */

//Clear screen
void OP_00E0(){
  logput("Executing 00E0\n");
  logopcode();
  memset(video, 0, sizeof(video));
}

//Return from subroutine
void OP_00EE(){
  sp--;
  pc = stack[sp];
}

//Jump to location
void OP_1nnn(){
  logput("Executing 0x1nnn\n");
  logopcode();
  //Extract lower 12 bits
  uint16_t address = opcode & 0x0FFFu;
  pc = address;
}

//Call address
void OP_2nnn(){
  //Extract lower 12 bits
  uint16_t address = opcode & 0x0FFFu;
  stack[sp] = pc;
  sp++;
  pc = address;
}

//Skip next instruction if Vx = kk
void OP_3xkk(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t byte = opcode & 0x00FFu;

  if(registers[Vx] == byte){
    pc += 2;
  }
}

//Skip next instruction if Vx != kk
void OP_4xkk(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t byte = opcode & 0x00ffu;

  if(registers[Vx] != byte){
    pc +=2;
  }
}

//Skip next instruction if Vx = Vy
void OP_5xy0(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t Vy = (opcode & 0x00f0u) >> 4u;

  if(registers[Vx] == registers[Vy]){
    pc += 2;
  }
}

//Set Vx == kk
void OP_6xkk(){
  logput("Executing 6xkk\n");
  logopcode();
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t byte = (opcode & 0x00FFu);
  registers[Vx] = byte;
}

//Add Vx and kk;
void OP_7xkk(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  uint8_t byte = (opcode & 0x00FFu);
  registers[Vx] += byte;
}

//Load Vy into Vx
void OP_8xy0(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t Vy = (opcode & 0x00f0u) >> 4u;
  registers[Vx] = registers[Vy];
}

//Vx = Vx OR Vy
void OP_8xy1(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t Vy = (opcode & 0x00f0u) >> 4u;
  registers[Vx] |= registers[Vy];
}

//Vx = Vx AND Vy
void OP_8xy2(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t Vy = (opcode & 0x00f0u) >> 4u;
  registers[Vx] &= registers[Vy];
}

//Vx = Vx XOR Vy
void OP_8xy3(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t Vy = (opcode & 0x00f0u) >> 4u;
  registers[Vx] ^= registers[Vy];
}

//Vx = Vx + Vy. If calculation overflows, lowest 8 bits stored and VF = 1
void OP_8xy4(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t Vy = (opcode & 0x00f0u) >> 4u;

  uint16_t sum = registers[Vx] + registers[Vy];

  if(sum > 255u){
    registers[0xF] = 1;
  } else {
    registers[0xf] = 0;
  }

  registers[Vx] = sum & 0xffu;
}

//Vx = Vx - Vy, set VF to NOT borrow (if Vx > Vy, VF = 1)
void OP_8xy5(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t Vy = (opcode & 0x00f0u) >> 4u;

  if(registers[Vx] > registers[Vy]){
    registers[0xf] = 1;
  } else {
    registers[0xf] = 0;
  }

  registers[Vx] -= registers[Vy];
}

//Right hand shifts Vx by 1. If the LSB is 1, then VF is set to 1.
void OP_8xy6(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;

  registers[0xf] = (registers[Vx] & 0x1u);

  registers[Vx] >>= 1;
}

//Vx = Vy - Vx, set VF to NOT borrow (if Vy > Vx, VF = 1)
void OP_8xy7(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t Vy = (opcode & 0x00f0u) >> 4u;

  if(registers[Vy] > registers[Vx]){
    registers[0xf] = 1;
  } else {
    registers[0xf] = 0;
  }

  registers[Vx] = registers[Vy] - registers[Vx];
}

//Left hand shifts Vx by 1. If the RSB is 1, then VF is set to 1.
void OP_8xyE(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;

  registers[0xf] = (registers[Vx] & 0x80u) >> 7u;

  registers[Vx] <<= 1;
}

//Skip next instruction if Vx != Vy
void OP_9xy0(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t Vy = (opcode & 0x00f0u) >> 4u;

  if(registers[Vx] != registers[Vy]){
    pc+=2;
  }
}

//Set I = nnn
void OP_Annn(){
  logput("Executing Annn\n");
  logopcode();
  uint16_t address = opcode & 0x0FFFu;

	memindex = address;
}

//Jump to location nnn + V0
void OP_Bnnn(){
  uint16_t address = opcode & 0x0FFFu;

	pc = registers[0] + address;
}

//Set Vx = random byte AND kk
void OP_Cxkk(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = randByte() & byte;
}

//Draw n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
void OP_Dxyn(){
  logput("Executing Dxyn\n");
  logopcode();
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

  registers[0xF] = 0;

  for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[memindex + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel)
			{
        logput("Sprite pixel was on, pixel flipped\n");
        logput("Editing %d element value: ", (yPos + row) * VIDEO_WIDTH + (xPos + col));
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					registers[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
        logput("%x\n", *screenPixel);
			}
		}
	}
}

//Skip next instruction if key with value of Vx is pressed
void OP_Ex9E(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;

  uint8_t key = registers[Vx];

  if(keypada[key]){
    pc+=2;
  }
}

//Skip next instruction if key with value of Vx is not pressed
void OP_ExA1(){
  uint8_t Vx = (opcode & 0xf00u) >> 8u;

  uint8_t key = registers[Vx];

  if(!keypada[key]){
    pc += 2;
  }
}

//Set Vx = delay timer value
void OP_Fx07(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;

  registers[Vx] = delayTimer;
}

//Wait until keypress and store in Vx
void OP_Fx0A(){
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (keypada[0])
	{
		registers[Vx] = 0;
	}
	else if (keypada[1])
	{
		registers[Vx] = 1;
	}
	else if (keypada[2])
	{
		registers[Vx] = 2;
	}
	else if (keypada[3])
	{
		registers[Vx] = 3;
	}
	else if (keypada[4])
	{
		registers[Vx] = 4;
	}
	else if (keypada[5])
	{
		registers[Vx] = 5;
	}
	else if (keypada[6])
	{
		registers[Vx] = 6;
	}
	else if (keypada[7])
	{
		registers[Vx] = 7;
	}
	else if (keypada[8])
	{
		registers[Vx] = 8;
	}
	else if (keypada[9])
	{
		registers[Vx] = 9;
	}
	else if (keypada[10])
	{
		registers[Vx] = 10;
	}
	else if (keypada[11])
	{
		registers[Vx] = 11;
	}
	else if (keypada[12])
	{
		registers[Vx] = 12;
	}
	else if (keypada[13])
	{
		registers[Vx] = 13;
	}
	else if (keypada[14])
	{
		registers[Vx] = 14;
	}
	else if (keypada[15])
	{
		registers[Vx] = 15;
	}
	else
	{
    //Effectively enters a loop, as pc will not increment beyond this instruction until key is pressed
		pc -= 2;
	}
  keyUsed = true;
}

//Set delay timer = Vx
void OP_Fx15(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;

  delayTimer = registers[Vx];
}

//Set sound timer = Vx
void OP_Fx18(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;

  soundTimer = registers[Vx];
}

//Set I = I + Vx
void OP_Fx1E(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  memindex += registers[Vx];
}

//Set I = sprite location for digit Vx
void OP_Fx29(){
  logput("Executing Fx29\n");
  logopcode();
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t digit = registers[Vx];

  memindex = FONTSET_START_ADDRESS + (5 * digit);
}

//Store binary coded decimal representation of Vx at I, I+1 and I+2
void OP_Fx33(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;
  uint8_t value = registers[Vx];

  //Ones place
  memory[memindex + 2] = value % 10;
  value /= 10;

  //Tens place
  memory[memindex + 1] = value % 10;
  value /= 10;

  //Hundreds place
  memory[memindex] = value % 10;
}

//Store registers V0 through Vx in memory starting at I
void OP_Fx55(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;

  uint8_t i;
  for(i = 0; i <= Vx; i++){
    memory[memindex + i] = registers[i];
  }
}

//Read registers V0 through Vx from memory starting at I
void OP_Fx65(){
  uint8_t Vx = (opcode & 0x0f00u) >> 8u;

  uint8_t i;
  for(i = 0; i <= Vx; i++){
    registers[i] = memory[memindex + i];
  }
}

//Instruction tables
Chip8Func table[0xf + 1];
Chip8Func table0[0xe + 1];
Chip8Func table8[0xe + 1];
Chip8Func tableE[0xe + 1];
Chip8Func tableF[0x65 + 1];

void initTables(){
  //Main table
  table[0x0] = &Table0;
  table[0x1] = &OP_1nnn;
  table[0x2] = &OP_2nnn;
  table[0x3] = &OP_3xkk;
  table[0x4] = &OP_4xkk;
  table[0x5] = &OP_5xy0;
  table[0x6] = &OP_6xkk;
  table[0x7] = &OP_7xkk;
  table[0x8] = &Table8;
  table[0x9] = &OP_9xy0;
  table[0xA] = &OP_Annn;
  table[0xB] = &OP_Bnnn;
  table[0xC] = &OP_Cxkk;
  table[0xD] = &OP_Dxyn;
  table[0xE] = &TableE;
  table[0xF] = &TableF;

  //Table0
  //Issue is here I'm pretty sure
  table0[0x0] = &OP_00E0;
  table0[0xE] = &OP_00EE;

  //Table8
  table8[0x0] = &OP_8xy0;
  table8[0x1] = &OP_8xy1;
  table8[0x2] = &OP_8xy2;
  table8[0x3] = &OP_8xy3;
  table8[0x4] = &OP_8xy4;
  table8[0x5] = &OP_8xy5;
  table8[0x6] = &OP_8xy6;
  table8[0x7] = &OP_8xy7;
  table8[0xE] = &OP_8xyE;

  //TableE
  //Even though opcodes are a full byte, they can be differentiated by lower nibble
  tableE[0x1] = &OP_ExA1;
  tableE[0xe] = &OP_Ex9E;

  //TableF
  tableF[0x07] = &OP_Fx07;
  tableF[0x0A] = &OP_Fx0A;
  tableF[0x15] = &OP_Fx15;
  tableF[0x18] = &OP_Fx18;
  tableF[0x1E] = &OP_Fx1E;
  tableF[0x29] = &OP_Fx29;
  tableF[0x33] = &OP_Fx33;
  tableF[0x55] = &OP_Fx55;
  tableF[0x65] = &OP_Fx65;
}



void Table0(){
  //Ensures second nibble is E
  if(((opcode & 0xf0u) >> 4u) == 0xE){
    table0[opcode & 0x000fu]();
  }
}

void Table8(){
  table8[opcode & 0x000fu]();
}

void TableE(){
  tableE[opcode & 0x000fu]();
}

void TableF(){
  tableF[opcode & 0x00ffu]();
}