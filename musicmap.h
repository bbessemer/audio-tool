#pragma once
#include <slice.h>

#define NOTE(midi) (midi % 12)
#define OCTAVE(midi) ((int)midi/12 - 1)
#define MIDI(note,octave) (((octave + 1) * 12) + note)

// Scale modes
#define MAJOR 0
#define DORIAN 1
#define PHRYGIAN 2
#define LYDIAN 3
#define MIXOLYDIAN 4
#define MINOR 5
#define LOCRIAN 6
#define HARMONIC 7

// Interval qualifications
#define PERFECT 0
#define AUGMENTED 1
#define DIMINISHED -1

#define NO_ADDITIONS 0x00
#define ADD_SECOND  0x02
#define ADD_FOURTH  0x08
#define ADD_SEVENTH 0x40

#define _WHOLE     384
#define _HALF      192
#define _QUARTER   96
#define _8TH       48
#define _16TH      24
#define _32ND      12

#define USE_SHARPS 1
#define USE_FLATS -1
#define USE_DEFAULT 0

#define BACKGROUND_COLOR {255,255,255,255}

#define CHANNELS (7*OCTAVES+1)
#define ROLL_HEIGHT ((OCTAVES > 2) ? 0.7 : 0.3*OCTAVES)
#define ROLL_WIDTH 0.3
#define GRID_COLOR {0,0,0,255}
#define MEASURE_BAR_COLOR {0,0,191,255}
#define MINIMEASURE_BAR_COLOR {159,159,255,255}
#define MARKER_COLOR {255,0,0,255}

#define BLACK   {0,   0,   0,   255}
#define WHITE   {255, 255, 255, 255}
#define RED     {255, 0,   0,   255}
#define ORANGE  {255, 127, 0,   255}
#define YELLOW  {255, 255, 0,   255}
#define GREEN   {0,   255, 0,   255}
#define BLUE    {0,   0,   255, 255}
#define PURPLE  {127, 0,   127, 255}
#define MAGENTA {255, 0,   255, 255}
#define GREY    {127, 127, 127, 255}

#define MELODY_NOTE 1
#define HARMONY_NOTE 2

struct Note
{
	slScalar pitch;
	slBS channel;
	slBS accidental;
	slScalar start,duration;
	slBox* box;
	slBU instrument;
	slScalar volume;
  slBU type;
  slBox* instrument_box;
};

Note* SpawnNote ();
Note* SpawnHiddenNote ();
void DespawnNote (Note* todespawn);
void RecalculateNotePitch(Note* note, slBS scale = MAJOR);
void RepositionNotes ();
void DrawGrid (SDL_Window*, SDL_Renderer*);
void DrawBackground (SDL_Window*, SDL_Renderer*);
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
void SetMasterVolume (slScalar);
char* GetNoteName (int, int);
int GetKeyNote ();
void SetKeyNote (int);
void CreateNoteLengthBind (char* name, Uint32 dflt_key, slScalar length);
slScalar SongProgress ();
void SetSongProgress (slScalar to);
