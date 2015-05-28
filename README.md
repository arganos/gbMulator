# gbmulator
A tiny emulator of the Nintendo Game Boy. Why? Because I decided to write an emulator. Why the Game Boy? Because its hardware is probably the least complicated around there is a lot of reference material on the internet.

It's being developed on Mac OS X, however the program should remain portable thanks to the SFML, which is used to emulate the display, the input and hopefully someday the sound.

Sources: Game BoyTM CPU Manual by Pan of Anthrox, GABY, Marat Fayzullin, Pascal Felber, Paul Robson, Martin Korth, kOOPa, Bowser

###Milestones:

Opcodes implemented
CPU finalized (cpu_instrs.gb 100%, instr_timing.gb 100%)
Display (Background, Window and Sprites seem to work. OAM Bug not implemented)
Input
MBC1

###Future Goals:

Save states
MBC 3/5/7 Hu1/3
Intern Debugger <---------
Color Game Boy support | NOT contradictory ;-)
Less Bugs inside <---------
Sound Support
Link Cable Support (Pipe? Online?)

###Games tested to work:

Tetris, Pokemon Red, Zelda: Link's Awakening.
