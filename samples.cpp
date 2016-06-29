#include <slice.h>
#include "samples.h"
#include "musicmap.h"

Instrument* Instruments = NULL;
slBU InstrumentCount = 0;
void LoadInstrument (char* name, char* from, slBU refnote, char* iconpath)
{
	slSoundSource* src = slLoadSoundSource(from);
	if (src)
	{
		// a sound needs to reference the source
		// otherwise the engine will GC the source
		slSound* container = slCreateSound(src);
		container->playing = false; // don't play (yet)
		container->destroy = false; // don't GC when not playing
		Instruments = realloc(Instruments,sizeof(Instrument) * (InstrumentCount + 1));
		Instrument* inst = Instruments + InstrumentCount;
		InstrumentCount++;
		inst->name = name;
		inst->sound = container;
		inst->refnote = refnote;
		inst->refpitch = GetPitch(refnote);
		inst->iconpath = iconpath;
	}
}
void LoadAllInstruments ()
{
	LoadInstrument("Piano", "samples/piano.swag", 69, "icons/piano.png"); // reference pitch is A4
	LoadInstrument("Strings", "samples/strings.swag", 72, "icons/strings.png"); // reference pitch is C5
  LoadInstrument("Flute", "samples/flute.swag", 69, "icons/flute.png");
  LoadInstrument("Trumpet", "samples/trumpet.swag", 69, "icons/trumpet.png");
  LoadInstrument("Solo Violin", "samples/violin.swag", 69, "icons/violin.png");
}
char* GetInstrumentName (slBU inst_id)
{
	if (inst_id >= InstrumentCount) return NULL;
	else return (Instruments + inst_id)->name;
}
slBU GetInstrumentID (char* name)
{
	slBU cur;
	for (cur = 0; cur < InstrumentCount; cur++)
		if (!strcmp((Instruments + cur)->name,name)) return cur;
	return cur; // InstrumentCount + 1
}
float GetSineSample (float freq, float pos)
{
	return sinf(pos * freq * M_PI);
}
float GetInstrumentSample (slBU inst_id, float freq, float offset)
{
	if (inst_id >= InstrumentCount) return GetSineSample(freq,offset);
	else
	{
		//return 0;
		Instrument* inst = Instruments + inst_id;
		if (inst->sound->src->ready && inst->sound->src->samples)
		{
			//printf("%f %f %f\n",offset,freq,inst->sound->src->persecond);
			slBU sampleoffset = slRound(offset * (freq / (inst->refpitch * 4)) * inst->sound->src->persecond);
			//printf("%lX\n",sampleoffset);
			while (sampleoffset >= inst->sound->src->samplecount) sampleoffset -= inst->sound->src->samplecount;
			if (inst->sound->src->samples_right) return (*(inst->sound->src->samples + sampleoffset) + *(inst->sound->src->samples_right + sampleoffset)) / 2;
			else return *(inst->sound->src->samples + sampleoffset);
		}
	}
}
slTexture* GetInstrumentIcon (slBU inst_id)
{
	if (inst_id >= InstrumentCount) return NULL;
	else
	{
		Instrument* inst = Instruments + inst_id;
		return slLoadTexture(inst->iconpath);
	};
};
