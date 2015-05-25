======================
CHIP8 emulator for 3DO 
======================

Port to 3DO by gameblala

This is a port of a CHIP8 emulator to the 3DO.
You need to bin2c your ROM and re-compile from scratch if you want to play another game other than BRIX.

The emulator runs fullspeed on 3DO.
In fact, it's a little too fast.

The binary has the BRIX game included with.

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