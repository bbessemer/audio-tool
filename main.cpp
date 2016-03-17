#include <slice.h>
#include <sliceopts.h>

#include "musicmap.h"
#include "chords.h"
#include "samples.h"
#include "interface.h"
#include "sliceexts.h"
#include "note-volume.h"

void InsertMeasureAtEnd ()
{
	InsertMeasure(GetMeasureCount());
}
void RemoveMeasureFromEnd ()
{
	RemoveMeasure(GetMeasureCount() - 1);
}
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
}
void ToggleLooping ()
{
	LoopSong = !LoopSong;
	LoopToggleButton->texref = slLoadTexture(LoopSong ? NOLOOP_BUTTON_IMGPATH : LOOP_BUTTON_IMGPATH);
}

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
			}
		}
	}
	if (SongPlaying)
	{
		for (cur = 0; cur < samples; cur++)
		{
			// Get a sample from the MusicMap.
			slScalar value = GetMixerSample(persample);
			// Copy this sample into the audio.
			*(buf + cur) = value;
			// Since we're only making mono sound,
			// the right channel in stereo sound
			// should have the same data as left.
			if (stereo) *(buf + samples + cur) = value;
		}
	}
	else
	{
		for (cur = 0; cur < samples; cur++) *(buf + cur) = 0;
		if (stereo) for (cur = 0; cur < samples; cur++) *(buf + samples + cur) = 0;
	}
}

int main ()
{
	slInit("EasyAudio", "app-icon.png");
	opInit(EnableOptionsMenu);

	// UI
	PlayToggleButton = slCreateBox(slLoadTexture(PLAY_BUTTON_IMGPATH));
	slSetBoxDims(PlayToggleButton,0.42,0.87,0.06,0.06);
	PlayToggleButton->z = 9;
	PlayToggleButton->onclick = TogglePlaying;
	LoopToggleButton = slCreateBox(slLoadTexture(LOOP_BUTTON_IMGPATH));
	slSetBoxDims(LoopToggleButton,0.52,0.87,0.06,0.06);
	LoopToggleButton->z = 9;
	LoopToggleButton->onclick = ToggleLooping;

	// Initialization
	slSetCustomDrawStage_Back(DrawBackground);
	slSetCustomDrawStage_Middle(DrawGrid);
	slOpenAudio();
	slSetCustomMixStage(Mix);
	VolAdjInit();

	// Keybindings
	slGetKeyBind("Append Measure to End",SDLK_m)->onpress = InsertMeasureAtEnd;
	slGetKeyBind("Chop Measure from End",SDLK_n)->onpress = RemoveMeasureFromEnd;
	slKeyBind* dragbind = slGetKeyBind("Drag Note",0,SDL_BUTTON(SDL_BUTTON_LEFT));
	dragbind->onpress = GrabNote;
	dragbind->onrelease = ReleaseNote;
	slGetKeyBind("Toggle Looping",SDLK_l)->onpress = ToggleLooping;
	slGetKeyBind("Toggle Playing",SDLK_SPACE)->onpress = TogglePlaying;
	slGetKeyBind("Whole Note", SDLK_a)->onpress = NoteLengthKeyBind;
	slGetKeyBind("Half Note", SDLK_s)->onpress = NoteLengthKeyBind;
	slGetKeyBind("Quarter Note", SDLK_d)->onpress = NoteLengthKeyBind;
	slGetKeyBind("Eighth Note", SDLK_f)->onpress = NoteLengthKeyBind;
	slGetKeyBind("Sixteenth Note", SDLK_g)->onpress = NoteLengthKeyBind;
	slGetKeyBind("32nd Note", SDLK_h)->onpress = NoteLengthKeyBind;
	slGetKeyBind("I Chord", SDLK_1)->onpress = EditChordKeyBind;
	slGetKeyBind("II Chord", SDLK_2)->onpress = EditChordKeyBind;
	slGetKeyBind("III Chord", SDLK_3)->onpress = EditChordKeyBind;
	slGetKeyBind("IV Chord", SDLK_4)->onpress = EditChordKeyBind;
	slGetKeyBind("V Chord", SDLK_5)->onpress = EditChordKeyBind;
	slGetKeyBind("VI Chord", SDLK_6)->onpress = EditChordKeyBind;
	slGetKeyBind("VII Chord", SDLK_7)->onpress = EditChordKeyBind;
	slGetKeyBind("Remove Chord", SDLK_DELETE)->onpress = RemoveChordKeyBind;

	// Loading
	LoadAllInstruments();
	slxEnableFpsCounter(SDLK_F3);
    InsertMeasureAtEnd();

	// Main Loop
	while (!slGetReqt())
	{
		RepositionNotes();
		RepositionChords();
		UpdateGrabbedNote();
		slxRunHooks();
		VolAdjStep();
		slCycle();
	}

	// Cleanup
	VolAdjQuit();
	slDestroyBox(PlayToggleButton);
	slDestroyBox(LoopToggleButton);
	opQuit();
	slQuit();
	return 0;
}
