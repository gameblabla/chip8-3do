/*
Copyright (C) 2015	Gameblabla <gameblabla@openmailbox.org>

This work is licensed to you under the terms of the MIT license;
see file LICENSE or http://www.opensource.org/licenses/mit-license.php

This work is also using works (Liberis and its sample) done by Alex Marshall Copyright (C) 2011.
See file LICENSE-MARSHALL or http://www.opensource.org/licenses/mit-license.php
*/

u32 padtype, paddata;

#define JOYPAD_A paddata & (1 << 0)
#define JOYPAD_B paddata & (1 << 1)
#define JOYPAD_C paddata & (1 << 2)
#define JOYPAD_D paddata & (1 << 3)
#define JOYPAD_E paddata & (1 << 4)
#define JOYPAD_F paddata & (1 << 5)

#define JOYPAD_SELECT paddata & (1 << 6)
#define JOYPAD_START paddata & (1 << 7)

#define JOYPAD_UP paddata & (1 << 8)
#define JOYPAD_RIGHT paddata & (1 << 9)
#define JOYPAD_DOWN paddata & (1 << 10)
#define JOYPAD_LEFT paddata & (1 << 11)

void Controls();

void print(char* string, int x , int y , int size);

int rand(void);
void srand(unsigned int seed);
int rand_a_b(int a, int b);
char* itoa(int val);
