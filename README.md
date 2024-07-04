# gbMulator
A tiny emulator of the Nintendo Game Boy I wrote a while ago. Why specifically the Game Boy? Because its hardware has a good balance of being relatively simple to describe while still being interesting to learn about. Furthermore there is a lot of reference material on the internet.

It uses QT6 with OpenGL to emulate the display, the input, the GUI and hopefully someday the sound.

Sources: Game BoyTM CPU Manual by Pan of Anthrox, GABY, Marat Fayzullin, Pascal Felber, Paul Robson, Martin Korth, kOOPa, Bowser

## Milestones:

1. Opcodes implemented
2. CPU finalized (cpu_instrs.gb 100%, instr_timing.gb 100%)
3. Display (Background, Window and Sprites seem to work. OAM Bug not implemented)
4. Input
5. MBC1

## Future Goals:

1. Save states
2. MBC 2/3/5/7 Hu1/3
3. Internal Debugger 
4. Color Game Boy support 
6. Sound Support
7. Link Cable Support (Pipe? Online?)

## Current state:

It runs most simple games with use the memory bank layout MBC1. So in particular early Game Boy games will run, while the more complex later games are not always compatible. I tested some freely available homebrew games such as 2friendly (https://isacben.itch.io/2friendly), which do rather well.
![Alt text](https://github.com/denengelbert/gbMulator/blob/master/img/title.png?raw=true)

![Alt text](https://github.com/denengelbert/gbMulator/blob/master/img/gameplay.png?raw=true)

