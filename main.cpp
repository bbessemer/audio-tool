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
	if (ReachedEnd) SetSongPosition(0);
	ReachedEnd = false;
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

slBox* ControlsInfoBox;
void OnControlsPress ()
{
    ControlsInfoBox->visible = !ControlsInfoBox->visible;
}

void OnProgressAdjust (slSlider* slider)
{
	SetSongProgress(slider->curvalue);
};

extern float BeatsPerMinute;
void ChangeTempo (slSlider* slider)
{
	slBS tempo = slRound(slider->curvalue);
	slSetSliderValue(slider,tempo);
	BeatsPerMinute = tempo;
	char* bpm_str;
	asprintf(&bpm_str,"%d",(int)tempo);
	slider->mark->texref = slRenderText(bpm_str);
	free(bpm_str);
};

int main ()
{
	slInit("EasyAudio");
	opInit();

	// UI
	PlayToggleButton = slCreateBox(slLoadTexture(PLAY_BUTTON_IMGPATH));
	slSetBoxDims(PlayToggleButton,0.42,0.87,0.06,0.06);
	PlayToggleButton->z = 22;
	PlayToggleButton->onclick = TogglePlaying;
	LoopToggleButton = slCreateBox(slLoadTexture(LOOP_BUTTON_IMGPATH));
	slSetBoxDims(LoopToggleButton,0.52,0.87,0.06,0.06);
	LoopToggleButton->z = 22;
	LoopToggleButton->onclick = ToggleLooping;
	ControlsInfoBox = slCreateBox(slLoadTexture("interface/help-card.png"));
	slSetBoxDims(ControlsInfoBox,0.05,0.05,0.9,0.9);
	ControlsInfoBox->z = 18;
	ControlsInfoBox->texbordercolor = {0,0,0,255};
	ControlsInfoBox->onclick = slDoNothing;
	slBox* ControlsHintBox = slCreateBox(slRenderText("Press C to toggle controls info.",{0,0,0,255}));
	slSetBoxDims(ControlsHintBox,0.01,0,0.2,0.05);
	ControlsHintBox->z = 18;
	slBox* ProgressSliderBack = slCreateBox();
	slBox* ProgressSliderMark = slCreateBox();
	slSlider* ProgressSlider = slCreateSlider(ProgressSliderBack,ProgressSliderMark,false,true,false);
	ProgressSliderBack->backcolor = {159,159,159,255};
	ProgressSliderMark->backcolor = {63,63,63,255};
	ProgressSlider->minvalue = 0;
	ProgressSlider->maxvalue = 1;
	slSetBoxDims(ProgressSliderBack,0,0.97,1,0.03);
	slSetBoxDims(ProgressSliderMark,0,0.97,0,0.03);
	ProgressSliderBack->z = 210;
	ProgressSliderMark->z = 209;
	ProgressSlider->onchange = OnProgressAdjust;
	slBox* TempoSliderBack = slCreateBox();
	slBox* TempoSliderMark = slCreateBox();
	slSetBoxDims(TempoSliderBack,(0.42 - 0.35) / 2,0.87,0.35,0.06);
	slSetBoxDims(TempoSliderMark,0,0.87,0.05,0.06);
	slSlider* TempoSlider = slCreateSlider(TempoSliderBack,TempoSliderMark);
	TempoSliderBack->z = 210;
	TempoSliderMark->z = 209;
	TempoSliderBack->backcolor = {191,191,191,255};
	TempoSliderMark->backcolor = {95,95,95,255};
	TempoSlider->onchange = ChangeTempo;
	TempoSlider->minvalue = 40;
	TempoSlider->maxvalue = 220;
	TempoSlider->curvalue = 120.1;
	ChangeTempo(TempoSlider); // Initialize text on slider mark.
	slBox* TempoInfoBox = slCreateBox(slRenderText("TEMPO",BLACK));
	TempoInfoBox->backcolor = TempoSliderBack->backcolor;
	slSetBoxDims(TempoInfoBox,TempoSliderBack->x,0.84,0.07,0.03);

	// Initialization
	slSetCustomDrawStage_Back(DrawBackground);
	slSetCustomDrawStage_Middle(DrawGrid);
	slOpenAudio();
	slSetCustomMixStage(Mix);
	VolAdjInit();

	// Keybindings
	slGetKeyBind("Show/Hide Controls Info",SDLK_c)->onpress = OnControlsPress;
	slGetKeyBind("Append Measure to End",SDLK_m)->onpress = InsertMeasureAtEnd;
	slGetKeyBind("Chop Measure from End",SDLK_n)->onpress = RemoveMeasureFromEnd;
	slKeyBind* dragbind = slGetKeyBind("Drag Note",0,SDL_BUTTON(SDL_BUTTON_LEFT));
	dragbind->onpress = GrabNote;
	dragbind->onrelease = ReleaseNote;
	slGetKeyBind("Toggle Looping",SDLK_l)->onpress = ToggleLooping;
	slGetKeyBind("Toggle Playing",SDLK_SPACE)->onpress = TogglePlaying;
	CreateNoteLengthBind("Whole Note",SDLK_a,8);
	CreateNoteLengthBind("Half Note",SDLK_s,4);
	CreateNoteLengthBind("Quarter Note",SDLK_d,2);
	CreateNoteLengthBind("Eighth Note",SDLK_f,1);
	CreateNoteLengthBind("Sixteenth Note",SDLK_g,0.5);
	CreateNoteLengthBind("Thirty-Second Note",SDLK_h,0.25);
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
		slSetSliderValue(ProgressSlider,SongProgress());
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
