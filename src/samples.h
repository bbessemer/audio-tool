#pragma once
struct Instrument
{
    slSound* sound;
    char* name;
    slBU refnote;
    slScalar refpitch;
    char* iconpath;
};
float GetInstrumentSample (slBU inst_id, float freq, float offset);
void LoadAllInstruments();
slBU GetInstrumentID(char*);
slTexture* GetInstrumentIcon (slBU inst_id);

