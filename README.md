# Chip8
This is a Chip8 emulator built in pure C, designed to run entirely within terminal. I used [this C++ guide](https://austinmorlan.com/posts/chip8_emulator/) as a reference, though my implementation differs a little in the graphics/input implementation. Instead of SDL, I opted to use ncurses to stay within terminal and therefore had to update the input polling and the logic in some of the instructions. <br/><br/>
![gif](https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExdW05bXlsdHJxd2t1MXZuOTh4MHU5cjdjOWY1ZHB5enY4bnd1bzVnNSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/ENMIHf1aCXcFqfjEtd/giphy.gif)
## Usage
To execute from the command line, use: `chip8 <Delay> <ROM> (Debug)`. <br/>
`<Delay>` is a delay in ms, which controls the clock speed of the emulation. <br/>
`<ROM>` is the path to the ROM to be executed. ROMs can be [easily found online](https://github.com/dmatlack/chip8/tree/master/roms/games). <br/>
`(Debug)` is an optional argument which enables logging each opcode to a log.txt file. It is enabled for any value other than 0.
