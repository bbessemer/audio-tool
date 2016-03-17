#include "stdio.h"
#include "musicmap.h"

extern Note** Notes;
extern slBU NoteCount;

void SaveModuleFile (char* path)
{
    FILE* file = fopen(path,"w");
    if (!file)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"File I/O Error","Could not open the file for writing.",NULL);
        return;
    };
    fprintf(file,"%u\n",(unsigned int)NoteCount);
    for (slBU cur = 0; cur < NoteCount; cur++)
    {
        Note* note = *(Notes + cur);
        fprintf(
            file,
            "%f\n%d\n%d\n%f\n%f\n%u\n",
            (float)note->pitch,
            (int)note->channel,
            (int)note->accidental,
            (float)note->start,
            (float)note->duration,
            (unsigned int)note->instrument
        );
    };
};
void LoadModuleFile (char* from)
{
	FILE* file = fopen(from,"r");
	if (!file)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"File I/O Error","Could not open the file for reading.",NULL);
        return;
    };
    while (true)
    {
        float pitch;
        int channel;
        int accidental;
        float start;
        float duration;
        unsigned int instrument;
        char* numstr;

        if (!slGetFileLine(file,&numstr)) goto FAIL;
        if (!sscanf(numstr,"%f",&pitch)) goto FAIL_NUMSTR;
        free(numstr);

        if (!slGetFileLine(file,&numstr)) goto FAIL;
        if (!sscanf(numstr,"%d",&channel)) goto FAIL_NUMSTR;
        free(numstr);

        if (!slGetFileLine(file,&numstr)) goto FAIL;
        if (!sscanf(numstr,"%d",&accidental)) goto FAIL_NUMSTR;
        free(numstr);

        if (!slGetFileLine(file,&numstr)) goto FAIL;
        if (!sscanf(numstr,"%f",&start)) goto FAIL_NUMSTR;
        free(numstr);

        if (!slGetFileLine(file,&numstr)) goto FAIL;
        if (!sscanf(numstr,"%f",&duration)) goto FAIL_NUMSTR;
        free(numstr);

        if (!slGetFileLine(file,&numstr)) goto FAIL;
        if (!sscanf(numstr,"%u",&instrument)) goto FAIL_NUMSTR;
        free(numstr);

        Note* note = SpawnNote();
        note->pitch = pitch;
        note->channel = channel;
        note->accidental = accidental;
        note->start = start;
        note->duration = duration;
        note->instrument = instrument;

        continue;

        FAIL_NUMSTR:
        free(numstr);
        FAIL:
        break;
    };
};
