#define BEATS_PER_MEASURE 16
#define CHANNELS 8
#define ROLL_HEIGHT 0.5
#define ROLL_WIDTH 0.5
#define GRID_COLOR {191,191,191,255}
#define MEASURE_SEPARATOR_COLOR {255,0,255,255}
#define BEATS_PER_MINIMEASURE 4
#define MINIMEASURE_SEPARATOR_COLOR {223,223,0,255}

/** -------- **/

#include "musicmap.h"
slScalar CHANNEL_HEIGHT = ROLL_HEIGHT * (1. / CHANNELS);
slScalar BEAT_WIDTH = ROLL_WIDTH * (1. / BEATS_PER_MEASURE);
slScalar ROLL_TOP = (1 - ROLL_HEIGHT) / 2.;
slBU SongPosition = 0;
#define GetRollOffset() (SongPosition * BEAT_WIDTH)
#define GetRollLeft() (((1 - ROLL_WIDTH) / 2.) - GetRollOffset())
slBU MeasureCount = 0;
void DrawGrid ()
{
	printf("drawstage: %dx%dx%d\n",CHANNELS,BEATS_PER_MEASURE,MeasureCount);
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
Note* SpawnNote ()
{
	Note* out = malloc(sizeof(Note));
	out->box = slCreateBox();
	out->box->w = BEAT_WIDTH;
	out->box->h = CHANNEL_HEIGHT;
	out->box->backcolor = {rand() % 256,rand() % 256,rand() % 256,255};
	slAddItemToList(&Notes,&NoteCount,out);
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
void DespawnNote (Note* todespawn)
{
	slRemoveItemFromList(&Notes,&NoteCount,todespawn);
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
			break;
		};
	};
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
