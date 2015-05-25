/*
Copyright (C) 2015	Gameblabla <gameblabla@openmailbox.org>

This work is licensed to you under the terms of the MIT license;
see file LICENSE or http://www.opensource.org/licenses/mit-license.php

This work is also using works (Liberis and its sample) done by Alex Marshall Copyright (C) 2011.
See file LICENSE-MARSHALL or http://www.opensource.org/licenses/mit-license.php
*/

#include <eris/types.h>
#include <eris/std.h>
#include <eris/v810.h>
#include <eris/king.h>
#include <eris/tetsu.h>
#include <eris/romfont.h>
#include <eris/pad.h>

#include "functions.h"
#include "input.h"

void Controls()
{
	padtype = eris_pad_type(0);
	paddata = eris_pad_read(0);
}

void print(char* string, int x , int y , int size)
{	
	u32 str[256];

	chartou32(string, str);
	printstr(str, x, y+20, size);
}


static unsigned long int next = 1;

/*
	Simple LRNG.
	Liberis does not have one.
*/

int rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed)
{
    next = seed;
}

int rand_a_b(int a, int b)
{
    return rand()%(b-a) +a;
}

/*
	Simple itoa implementation
*/
char* itoa(int val)
{
	
	static char buf[32] = {0};
	int base = 10;
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}

