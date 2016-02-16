#ifndef SLX_H
#define SLX_H

// Defines slGetReqt for old versions of Slice.
#ifndef slGetReqt
#define slGetReqt(void) slGetExitReq()
#endif

// FPS Counter
void slxEnableFpsCounter (int toggle_key = 0, bool sticky = true);
void slxDisableFpsCounter ();
void slxToggleFpsCounter ();
void slxDestroyFpsCounter ();
slScalar slxGetFps ();

#endif
