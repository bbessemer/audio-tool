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
void DrawGrid (SDL_Window*, SDL_Renderer*);
slBU GetMeasureCount ();
void InsertMeasure (slBU where);
void RemoveMeasure (slBU where);
void GrabNote ();
void ReleaseNote ();
void UpdateGrabbedNote ();
float GetSample (slScalar persample);
