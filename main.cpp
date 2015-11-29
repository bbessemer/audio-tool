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
bool ReachedEnd = false; // Set by mixer controller.

// Toggles
bool SongPlaying = false;
bool LoopSong = false;
slBox* PlayToggleButton;
#define PLAY_BUTTON_IMGPATH "play-button.png"
#define PAUSE_BUTTON_IMGPATH "pause-button.png"
slBox* LoopToggleButton;
#define LOOP_BUTTON_IMGPATH "loop-button.png"
#define NOLOOP_BUTTON_IMGPATH "noloop-button.png"
void TogglePlaying ()
{
	if (!LoopSong && ReachedEnd) SetSongPosition(0);
	else ReachedEnd = false;
	SongPlaying = !SongPlaying;
	PlayToggleButton->texref = slLoadTexture(SongPlaying ? PAUSE_BUTTON_IMGPATH : PLAY_BUTTON_IMGPATH);
};
void ToggleLooping ()
{
	LoopSong = !LoopSong;
	LoopToggleButton->texref = slLoadTexture(LoopSong ? NOLOOP_BUTTON_IMGPATH : LOOP_BUTTON_IMGPATH);
};

// Mixer Controller
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
				TogglePlaying();
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

int main ()
{
	slInit();

	// UI
	PlayToggleButton = slCreateBox(slLoadTexture(PLAY_BUTTON_IMGPATH));
	slSetBoxDims(PlayToggleButton,0.42,0.72,0.06,0.06);
	PlayToggleButton->onclick = TogglePlaying;
	LoopToggleButton = slCreateBox(slLoadTexture(LOOP_BUTTON_IMGPATH));
	slSetBoxDims(LoopToggleButton,0.52,0.72,0.06,0.06);
	LoopToggleButton->onclick = ToggleLooping;

	// Initialization
	slSetCustomDrawStage_Middle(DrawGrid);
	slOpenAudio();
	slSetCustomMixStage(Mix);

	// Keybindings
	slGetKeyBind("Append Measure to End",SDLK_m)->onpress = InsertMeasureAtEnd;
	slGetKeyBind("Chop Measure from End",SDLK_n)->onpress = RemoveMeasureFromEnd;
	slKeyBind* dragbind = slGetKeyBind("Drag Note",0,SDL_BUTTON(SDL_BUTTON_LEFT));
	dragbind->onpress = GrabNote;
	dragbind->onrelease = ReleaseNote;
	slGetKeyBind("Toggle Looping",SDLK_l)->onpress = ToggleLooping;
	slGetKeyBind("Toggle Playing",SDLK_SPACE)->onpress = TogglePlaying;
	slGetKeyBind("Note Length 0", SDLK_h)->onpress = NoteLengthKeyBind;
	slGetKeyBind("Note Length 1", SDLK_g)->onpress = NoteLengthKeyBind;
	slGetKeyBind("Note Length 2", SDLK_f)->onpress = NoteLengthKeyBind;
	slGetKeyBind("Note Length 3", SDLK_d)->onpress = NoteLengthKeyBind;
	slGetKeyBind("Note Length 4", SDLK_s)->onpress = NoteLengthKeyBind;
	slGetKeyBind("Note Length 5", SDLK_a)->onpress = NoteLengthKeyBind;

	// Main Loop
	while (!slGetReqt())
	{
		RepositionNotes();
		UpdateGrabbedNote();
		slCycle();
	};

	// Cleanup
	slDestroyBox(PlayToggleButton);
	slDestroyBox(LoopToggleButton);
	slQuit();
};
