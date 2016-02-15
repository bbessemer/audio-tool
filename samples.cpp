#include <slice.h>
#include "samples.h"
Instrument* Instruments = NULL;
slBU InstrumentCount = 0;
void LoadInstrument (char* name, char* from)
{
	slSoundSource* src = slLoadSoundSource(from);
	if (src)
	{
		// a sound needs to reference the source
		// otherwise the engine will GC the source
		slSound* container = slCreateSound(src);
		container->playing = false; // don't play, but also...
		container->destroy = false; // don't GC when not playing
		Instruments = realloc(Instruments,sizeof(Instrument) * (InstrumentCount + 1));
		Instrument* inst = Instruments + InstrumentCount;
		InstrumentCount++;
		inst->name = name;
		inst->sound = container;
	};
};
void LoadAllInstruments ()
{
	LoadInstrument("Piano","samples/piano.swag");
};
char* GetInstrumentName (slBU inst_id)
{
	if (inst_id >= InstrumentCount) return NULL;
	else return (Instruments + inst_id)->name;
};
slBU GetInstrumentID (char* name)
{
	slBU cur;
	for (cur = 0; cur < InstrumentCount; cur++)
		if (!strcmp((Instruments + cur)->name,name)) return cur;
	return cur; // InstrumentCount + 1
};
float GetSineSample (float freq, float pos)
{
	return sinf(pos * freq * M_PI);
};
float GetInstrumentSample (slBU inst_id, float freq, float offset)
{
	if (inst_id >= InstrumentCount) return GetSineSample(freq,offset);
	else
	{
		//return 0;
		Instrument* inst = Instruments + inst_id;
		//printf("%f %f %f\n",offset,freq,inst->sound->src->persecond);
		slBU sampleoffset = slRound(offset * (freq / (440 * 4)) * inst->sound->src->persecond);
		//printf("%lX\n",sampleoffset);
		while (sampleoffset >= inst->sound->src->samplecount) sampleoffset -= inst->sound->src->samplecount;
		if (inst->sound->src->samples_right) return (*(inst->sound->src->samples + sampleoffset) + *(inst->sound->src->samples_right + sampleoffset)) / 2;
		else return *(inst->sound->src->samples + sampleoffset);
	};
};
