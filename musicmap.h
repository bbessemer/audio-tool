#pragma once
#include <slice.h>
struct Note
{
	int pitch,rel_pitch;
	slBU start,duration,channel;
	slBox* box;
	// TO DO
	/*
	Instrument** instrument;
	slScalar volume;
	*/
};
Note* SpawnNote ();
void RepositionNotes ();
void DrawGrid ();
