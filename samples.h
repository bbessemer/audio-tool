#pragma once
struct Instrument
{
	slSound* sound;
	char* name;
}
float GetInstrumentSample (slBU inst_id, float freq, float offset);
void LoadAllInstruments();
slBU GetInstrumentID(char*);
