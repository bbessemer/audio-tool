#define BEATS_PER_MEASURE 16
#define CHANNELS 8
#define ROLL_HEIGHT 0.5
#define ROLL_WIDTH 0.5
#define GRID_COLOR {255,255,255,255}

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
	slScalar roll_left = GetRollLeft();
	slScalar roll_right = roll_left + (MeasureCount * BEATS_PER_MEASURE * BEAT_WIDTH);
	slScalar roll_bottom = ROLL_TOP + (CHANNELS * CHANNEL_HEIGHT);
	for (slBU meas = 0; meas < MeasureCount; meas++) for (slBU beat = 0; beat < BEATS_PER_MEASURE; beat++)
	{
		slScalar x = roll_left + (((meas * BEATS_PER_MEASURE) + beat) * BEAT_WIDTH);
		slDrawScreenLine(x,ROLL_TOP,x,roll_bottom);
	};
	for (slBU chan = 0; chan < CHANNELS; chan++)
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
			slRemoveItemFromList(&Notes,&NoteCount,GrabbedNote);
			free(GrabbedNote);
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
