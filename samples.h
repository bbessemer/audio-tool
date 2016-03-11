#pragma once
struct Instrument
{
	slSound* sound;
	char* name;
	slBU refnote;
	// TODO: not all samples are by default playing the same note.
	// This needs to be corrected for in the mixer.
};
float GetInstrumentSample (slBU inst_id, float freq, float offset);
void LoadAllInstruments();
slBU GetInstrumentID(char*);
