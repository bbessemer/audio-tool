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
