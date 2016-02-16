#pragma once
#include <slice.h>
//#include "chords.h"

#define NOTE(midi) (midi % 12)
#define OCTAVE(midi) ((int)midi/12 - 1)
#define MIDI(note,octave) (((octave + 1) * 12) + note)

#define _WHOLE     384
#define _HALF      192
#define _QUARTER   96
#define _8TH       48
#define _16TH      24
#define _32ND      12

#define CHANNELS (7*OCTAVES+1)
#define ROLL_HEIGHT ((OCTAVES > 2) ? 0.7 : 0.3*OCTAVES)
#define ROLL_WIDTH 0.3
#define GRID_COLOR {191,191,191,255}
#define MEASURE_SEPARATOR_COLOR {255,0,255,255}
#define MINIMEASURE_SEPARATOR_COLOR {223,223,0,255}

#define BLACK   {0,   0,   0,   255}
#define WHITE   {255, 255, 255, 255}
#define RED     {255, 0,   0,   255}
#define ORANGE  {255, 127, 0,   255}
#define YELLOW  {255, 255, 0,   255}
#define GREEN   {0,   255, 0,   255}
#define BLUE    {0,   0,   255, 255}
#define PURPLE  {127, 0,   127, 255}
#define MAGENTA {255, 0,   255, 255}

struct Note
{
	slScalar pitch;
	slBS channel;
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
slScalar GetRollOffset ();
slScalar GetRollLeft ();
int GetBeatsPerMeasure ();
slScalar GetDefaultNoteLength ();
slScalar GetNoteLength ();
slScalar GetBeatWidth ();
SDL_Color GetNoteColor (int);
