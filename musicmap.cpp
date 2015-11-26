#define NOTE(midi) (midi % 12)
#define OCTAVE(midi) ((int)midi/12 - 1)
#define MIDI(note, octave) ((octave+1)*12+note)

#define _WHOLE     384
#define _HALF      192
#define _QUARTER   96
#define _8TH       48
#define _16TH      24
#define _32ND      12

#define CHANNELS 7
#define ROLL_HEIGHT 0.3
#define ROLL_WIDTH 0.5
#define GRID_COLOR {191,191,191,255}
#define MEASURE_SEPARATOR_COLOR {255,0,255,255}
#define MINIMEASURE_SEPARATOR_COLOR {223,223,0,255}

#define BLACK   {0,   0,   0,   255}
#define WHITE   {255, 255, 255, 255}
#define RED     {255, 0,   0,   255}
#define ORANGE  {255, 128, 0,   255}
#define YELLOW  {255, 255, 0,   255}
#define GREEN   {0,   255, 0,   255}
#define BLUE    {0,   0,   255, 255}
#define PURPLE  {128, 0,   128, 255}
#define MAGENTA {255, 0,   255, 255}

/** -------- **/

#include <stdlib.h>
#include "musicmap.h"
int BEATS_PER_MEASURE = 16;
int BEATS_PER_MINIMEASURE = 4;
int BEAT_LENGTH = _QUARTER;
int DEFAULT_NOTE_LENGTH = 1;
slScalar CHANNEL_HEIGHT = ROLL_HEIGHT * (1. / CHANNELS);
slScalar BEAT_WIDTH = ROLL_WIDTH * (1. / BEATS_PER_MEASURE);
slScalar ROLL_TOP = (1 - ROLL_HEIGHT) / 2.;
slScalar SongPosition = 0;
#define GetRollOffset() (SongPosition * BEAT_WIDTH)
#define GetRollLeft() (((1 - ROLL_WIDTH) / 2.) - GetRollOffset())
slBU MeasureCount = 0;
void DrawGrid (SDL_Window* window = NULL, SDL_Renderer* renderer = NULL)
{
	slScalar roll_left = GetRollLeft();
	slScalar roll_right = roll_left + (MeasureCount * BEATS_PER_MEASURE * BEAT_WIDTH);
	slScalar roll_bottom = ROLL_TOP + (CHANNELS * CHANNEL_HEIGHT);
	slBU beatcount = MeasureCount * BEATS_PER_MEASURE;
	for (slBU beat = 0; beat <= beatcount; beat++)
	{
		slScalar x = roll_left + (beat * BEAT_WIDTH);
		if (beat % BEATS_PER_MINIMEASURE) slSetDrawColor(GRID_COLOR);
		else if (beat % BEATS_PER_MEASURE) slSetDrawColor(MINIMEASURE_SEPARATOR_COLOR);
		else slSetDrawColor(MEASURE_SEPARATOR_COLOR);
		slDrawScreenLine(x,ROLL_TOP,x,roll_bottom);
	};
	slSetDrawColor(GRID_COLOR);
	for (slBU chan = 0; chan <= CHANNELS; chan++)
	{
		slScalar y = ROLL_TOP + (chan * CHANNEL_HEIGHT);
		slDrawScreenLine(roll_left,y,roll_right,y);
	};
};

Note** Notes = NULL;
slBU NoteCount = 0;

SDL_Color GetNoteColor (int relative)
{
  switch (relative)
  {
    case 0: return RED;
    case 1: return ORANGE;
    case 2: return YELLOW;
    case 3: return GREEN;
    case 4: return BLUE;
    case 5: return PURPLE;
    case 6: return MAGENTA;
    default: return WHITE;
  };
};

Note* SpawnNote ()
{
	Note* out = (Note *) malloc(sizeof(Note));
	out->box = slCreateBox();
	out->box->w = BEAT_WIDTH;
	out->box->h = CHANNEL_HEIGHT;
	out->box->bordercolor = BLACK;
	out->box->backcolor = {rand() % 256,rand() % 256,rand() % 256,255};
	slAddItemToList((void ***)&Notes, (Uint64 *)&NoteCount, (void *)out);
	return out;
};

void RepositionNotes ()
{
	slScalar roll_left = GetRollLeft();
	for (slBU cur = 0; cur < NoteCount; cur++)
	{
		Note* note = *(Notes + cur);
		note->box->x = roll_left + (note->start * BEAT_WIDTH);
		note->box->y = ROLL_TOP + (note->channel * CHANNEL_HEIGHT);
	};
};

void NewNoteAtClickPoint ()
{
	slScalar x, y;
	slGetMouse(&x, &y);
  // u and v represent coordinates in beats and channels, respectively.
  int u = (int)((x - GetRollLeft())/BEAT_WIDTH);
  int v = (int)((ROLL_TOP - y)/CHANNEL_HEIGHT) - 1;
  /* v will typically be negative because the baseline is the *top* of the
   * (original) roll; if the roll is extended upward the baseline will remain
	 * the same and v can be positive. */
	Note* note = SpawnNote();
	note->start = u;
	note->channel = v;
	note->duration = DEFAULT_NOTE_LENGTH;
	note->box->w = DEFAULT_NOTE_LENGTH*BEAT_WIDTH;
	note->box->x = GetRollLeft() + u*BEAT_WIDTH;
	note->box->y = ROLL_TOP - (slScalar)(v+1)*CHANNEL_HEIGHT;
	printf("%f\n", note->box->y);
	note->box->backcolor = GetNoteColor((v+7000) % 7);
}

void DespawnNote (Note* todespawn)
{
	slRemoveItemFromList((void ***)&Notes,&NoteCount,todespawn);
	free(todespawn);
};
Note* GrabbedNote = NULL;
void GrabNote ()
{
	slScalar mousex,mousey;
	slGetMouse(&mousex,&mousey);
	for (slBU cur = 0; cur < NoteCount; cur++)
	{
		Note* note = *(Notes + cur);
		if (slPointOnBox(note->box,mousex,mousey))
		{
			GrabbedNote = note;
			return;
		};
	};
	// Will only be reached if no note is there to be dragged.
	NewNoteAtClickPoint();
};
void ReleaseNote ()
{
	if (GrabbedNote)
	{
		slScalar mousex,mousey;
		slGetMouse(&mousex,&mousey);
		slScalar channel = ((mousey - ROLL_TOP) / ROLL_HEIGHT) * CHANNELS;
		slScalar beat = ((mousex - GetRollLeft()) / ROLL_WIDTH) * BEATS_PER_MEASURE;
		if (channel < 0 || beat < 0 || channel >= CHANNELS || beat >= BEATS_PER_MEASURE * MeasureCount)
		{
			// Remove the note. It's been dragged into the margins.
			DespawnNote(GrabbedNote);
		}
		else
		{
			// The note has been dragged into a valid grid space.
			GrabbedNote->channel = channel;
			GrabbedNote->start = beat;
		};
		// Either remove the note or
		// set its start and channel.
		GrabbedNote = NULL;
	};
};
void UpdateGrabbedNote ()
{
	if (GrabbedNote) slGetMouse(&(GrabbedNote->box->x),&(GrabbedNote->box->y));
};
slBU GetMeasureCount ()
{
	return MeasureCount;
};
void InsertMeasure (slBU where)
{
	if (where < MeasureCount)
	{
		// We need to reposition the notes after the insertion point.
		for (slBU cur = 0; cur < NoteCount; cur++)
		{
			Note* item = *(Notes + cur);
			if (item->start > where * BEATS_PER_MEASURE) item->start += BEATS_PER_MEASURE;
		};
	};
	MeasureCount++;
};
void RemoveMeasure (slBU where)
{
	if (MeasureCount)
	{
		for (slBU cur = 0; cur < NoteCount; cur++)
		{
			Note* item = *(Notes + cur);
			if (item->start / BEATS_PER_MEASURE == where)
			{
				// Clear out the notes that were inside this measure.
				DespawnNote(item);
				cur--; // The list has shrunk due to despawning this item.
			}
			else if (item->start > where * BEATS_PER_MEASURE)\
			{
				// Reposition notes after this measure.
				item->start -= BEATS_PER_MEASURE;
			};
		};
		MeasureCount--;
	};
};
slScalar BeatsPerMinute = 144;
slScalar GetSongPosition ()
{
	return SongPosition;
};
void SetSongPosition (slScalar to)
{
	SongPosition = to;
};
slScalar GetSongLength ()
{
	return MeasureCount * BEATS_PER_MEASURE;
};
#define TEST_HERTZ 440 /* A */
float GetSample (slScalar persample)
{
	slScalar sample = 0;
	sample += sin((SongPosition * (TEST_HERTZ / (BeatsPerMinute / 60))) * M_PI) * 0.5;
	// Get the sample.
	SongPosition += persample * (BeatsPerMinute / 60) * (BEATS_PER_MINIMEASURE);
	return sample;
};

