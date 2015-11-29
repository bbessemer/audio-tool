#pragma once
struct Instrument
{
	float persample;
	slSound* sound;
	char* name;
};
float GetInstrumentSample (slBU inst_id, float freq, float offset);
void LoadAllInstruments();
slBU GetInstrumentID(char*);
