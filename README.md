======================
CHIP8 emulator for 3DO 
======================

Port to 3DO by gameblala

This is a port of a CHIP8 emulator to the 3DO.

It is provided with its own game Tetris but you can swap it with your own chip8 game.
Simply replace the rom.ch8 rom file with your own and then use OperaFS,
tell it to use the "CD" directory and you'll end up with a ISO file to burn on your 3DO.

The emulator runs fullspeed on 3DO but it is sometimes bottleneck by the pixel drawing.

It was based on another chip8 emulator but it came with no license !
All the modifications to this chip8 emulator are licensed under the MIT license.

I'm also working on a port for NEC PC-FX.

=======
TODO
=======

- Add beeping sound
- A nice menu to configure the controls
- Save the configuration to NVRAM
- Eventually switch to another emulator supporting the MegaChip8 & SuperChip.


===================
RUN ON A REAL 3DO
===================

If you have a Goldstar then you will need good-quality CDs and you need to press them.
If you do not follow this requirement then it will probably not work.

To burn the iso to a CD, use ImgBurn. (ImgBurn also works on Wine)
Choose Burn ISO to DISC and choose chip8_emulator_3DO.iso.

The 3DO does not have copy protection so simply pop it in your disc drive
and you are good to go !