# gbmulator
A tiny emulator of the Nintendo Game Boy. Why? Because I decided to write an emulator. Why the Game Boy? Because its hardware is probably the least complicated around there is a lot of reference material on the internet.

It's being developed on Mac OS X, however the program should remain portable thanks to the SFML, which is used to emulate the display, the input and hopefully someday the sound.

Sources: Game BoyTM CPU Manual by Pan of Anthrox, GABY, Marat Fayzullin, Pascal Felber, Paul Robson, Martin Korth, kOOPa, Bowser

###Milestones:

1. Opcodes implemented
2. CPU finalized (cpu_instrs.gb 100%, instr_timing.gb 100%)
3. Display (Background, Window and Sprites seem to work. OAM Bug not implemented)
4. Input
5. MBC1

###Future Goals:

1. Save states
2. MBC 3/5/7 Hu1/3
3. Intern Debugger <---------
4. Color Game Boy support | NOT contradictory ;-)
5. Less Bugs inside <---------
6. Sound Support
7. Link Cable Support (Pipe? Online?)

###Games tested to work:

Tetris, Pokemon Red, Zelda: Link's Awakening.
