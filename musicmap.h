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
slBU GetMeasureCount ();
void InsertMeasure (slBU where);
void RemoveMeasure (slBU where);
void GrabNote ();
void ReleaseNote ();
void UpdateGrabbedNote ();
