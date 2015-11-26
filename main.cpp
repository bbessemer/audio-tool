#include <slice.h>

//#include "interface.h"
//#include "mixer/mixer.h"
#include "musicmap.h"

void InsertMeasureAtEnd ()
{
	InsertMeasure(GetMeasureCount());
};
void RemoveMeasureFromEnd ()
{
	RemoveMeasure(GetMeasureCount() - 1);
};
bool SongPlaying = false;
bool LoopSong = false;
void PlayPauseToggle ();
bool ReachedEnd = false;
void Mix (float* buf, slBU samples, bool stereo, slScalar persample)
{
	slBU cur;
	if (SongPlaying)
	{
		if (GetSongPosition() > GetSongLength())
		{
			if (LoopSong) SetSongPosition(0);
			else
			{
				PlayPauseToggle();
				SetSongPosition(GetSongLength());
				ReachedEnd = true;
			};
		};
	};
	if (SongPlaying)
	{
		for (cur = 0; cur < samples; cur++)
		{
			// Get a sample from the MusicMap.
			slScalar value = GetSample(persample);
			// Copy this sample into the audio.
			*(buf + cur) = value;
			// Since we're only making mono sound,
			// the right channel in stereo sound
			// should have the same data as left.
			if (stereo) *(buf + samples + cur) = value;
		};
	}
	else
	{
		for (cur = 0; cur < samples; cur++) *(buf + cur) = 0;
		if (stereo) for (cur = 0; cur < samples; cur++) *(buf + samples + cur) = 0;
	};
};
slBox* PlayPauseButton;
#define PLAY_BUTTON_IMGPATH "play-button.png"
#define PAUSE_BUTTON_IMGPATH "pause-button.png"
void PlayPauseToggle ()
{
	if (!LoopSong && ReachedEnd) SetSongPosition(0);
	else ReachedEnd = false;
	SongPlaying = !SongPlaying;
	PlayPauseButton->texref = slLoadTexture(SongPlaying ? PAUSE_BUTTON_IMGPATH : PLAY_BUTTON_IMGPATH);
};
int main ()
{
	slInit();
	PlayPauseButton = slCreateBox(slLoadTexture(PLAY_BUTTON_IMGPATH));
	slSetBoxDims(PlayPauseButton,0.42,0.72,0.06,0.06);
	PlayPauseButton->onclick = PlayPauseToggle;
	slSetCustomDrawStage_Middle(DrawGrid);
	slOpenAudio();
	slSetCustomMixStage(Mix);
	//ui_init();
	//music_init();
	slGetKeyBind("Append Measure to End",SDLK_m)->onpress = InsertMeasureAtEnd;
	slGetKeyBind("Chop Measure from End",SDLK_n)->onpress = RemoveMeasureFromEnd;
	slKeyBind* dragbind = slGetKeyBind("Drag Note",0,SDL_BUTTON(SDL_BUTTON_LEFT));
	dragbind->onpress = GrabNote;
	dragbind->onrelease = ReleaseNote;
	while (!slGetExitReq())
	{
		RepositionNotes();
		UpdateGrabbedNote();
		slCycle();
	};
	slQuit();
};
