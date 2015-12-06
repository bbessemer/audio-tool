#pragma once
#include <slice.h>
struct Note
{
	slScalar pitch;
	int rel_pitch;
	slBU channel;
	slScalar start,duration;
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
float GetMixerSample (slScalar persample);
slScalar GetSongPosition ();
slScalar GetSongLength ();
void SetSongPosition (slScalar to);
void NoteLengthKeyBind (slKeyBind* kb);
slScalar GetPitch (int midi_value);
