#include <revel.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>


// Function to draw a checkerboard pattern.  Don't worry about its details
// too much, it's not integral to the understanding of using revel.  We just
// need some interesting video frames to encode.
void DrawFrame(const int width, const int height, const unsigned int color,
               const int shiftX, const int shiftY, int *framebuffer)
{
    const int checksize = 32;
    const int colors[2] = {0xFFFFFFFF, color};
    int c = 0;
    int index = 0;
    for(int y=0; y<height; ++y)
    {
        if ((y+shiftY) % 32 == 0)
            c = 1-c;
        for(int x=0; x<width; ++x)
        {
            if ((x+shiftX) % 32 == 0)
                c = 1-c;
            framebuffer[index++] = colors[c];
        }
    }   
}


// Function to load a sound file for the sample movie's audio track.
// Once again, this is completely unrelated to using Revel, so don't
// worry about understanding it fully.
int audioBits = 16;
int audioChannels = 2;
int audioFormat = REVEL_ASF_PCM;
int audioRate = 22050;
void LoadAudio(bool *hasAudio, char **audioBuffer, int *audioBufferSize)
{
    // chimes.raw is raw sample data.  The details of its sample format are
    // hard-coded into this file, so that I don't have to clutter
    // up the example code with a WAV-loading function.
    // ENDIAN ISSUE: chimes.raw is stored in little-endian format.  Running
    // this code on a big-endian system won't crash, but the audio will sound
    // pretty awful.  You've been warned!
    const char *audioFilename = "chimes.raw";
    *hasAudio = false;
    FILE *audioFile = fopen(audioFilename, "rb");
    if (audioFile != NULL)
    {
        fseek(audioFile, 0, SEEK_END);
        *audioBufferSize = ftell(audioFile);
        fseek(audioFile, 0, SEEK_SET);
        *audioBuffer = new char[*audioBufferSize];
        *hasAudio = (fread(*audioBuffer, 1, *audioBufferSize, audioFile) == *audioBufferSize);
        fclose(audioFile);
    }
}

int InitializeEncoder(int width, int height)
{
    Revel_Error revError;

    // Make sure the API version of Revel we're compiling against matches the
    // header files!  This is terribly important!
    if (REVEL_API_VERSION != Revel_GetApiVersion())
    {
        printf("ERROR: Revel version mismatch!\n");
        printf("Headers: version %06x, API version %d\n", REVEL_VERSION,
            REVEL_API_VERSION);
        printf("Library: version %06x, API version %d\n", Revel_GetVersion(),
            Revel_GetApiVersion());
        return 0;
    }

    // Set up the encoding parameters.  ALWAYS call Revel_InitializeParams()
    // before filling in your application's parameters, to ensure that all
    // fields (especially ones that you may not know about) are initialized
    // to safe values.
    Revel_Params revParams;
    Revel_InitializeParams(&revParams);
    revParams.width = width;
    revParams.height = height;
    revParams.frameRate = 30.0f;
    revParams.quality = 1.0f;
    revParams.codec = REVEL_CD_XVID;
    revParams.hasAudio = 1; // It's okay to set this to 1 even if we don't call Revel_EncodeAudio() below.
    revParams.audioChannels = audioChannels;
    revParams.audioRate = audioRate;
    revParams.audioBits = audioBits;
    revParams.audioSampleFormat = audioFormat;

    // Create an encoder
    int encoderHandle;
    revError = Revel_CreateEncoder(&encoderHandle, &revParams);
    if (revError != REVEL_ERR_NONE)
    {
	    printf("Revel Error while creating encoder: %d\n", revError);
	    return 0;
    }

	return encoderHandle;
}

// Here, I'm just ensuring that Revel works even when called from global
// static initialization code; there's no reason that this handle (and the body of
// InitializeEncoder() above) could not be embedded directly into main().
int width = 320;
int height = 240;
static int encoderHandle = InitializeEncoder(width, height);

int main(int argc, char *argv[])
{
    const char *filename = "checkers.avi";
    int numFrames = 64;
	Revel_Error revError;

	// Make sure the encoder is valid
	if (!Revel_IsEncoderValid(encoderHandle))
	{
		return -1;
	}

    // Attempt to load some sound data, to encode into the output movie's
    // audio stream.
    bool hasAudio = false;
    char *audioBuffer = NULL;
	int audioBufferSize = 0;
    LoadAudio(&hasAudio, &audioBuffer, &audioBufferSize);
    if (!hasAudio)
    {
        printf("Warning: Failed to load audio test file: chimes.raw\n");
        printf("         The audio encoding tests will be skipped.\n");
    }

	// Initiate encoding
    revError = Revel_EncodeStart(encoderHandle, filename);
    if (revError != REVEL_ERR_NONE)
    {
	    printf("Revel Error while starting encoding: %d\n", revError);
	    exit(1);
    }

    // Draw and encode each frame.
    Revel_VideoFrame frame;
    frame.width = width;
    frame.height = height;
    frame.bytesPerPixel = 4;
    frame.pixelFormat = REVEL_PF_RGBA;
    frame.pixels = new int[width*height];
    memset(frame.pixels, 0, width*height*4);
    for(int i=0; i<numFrames; ++i)
    {
        DrawFrame(frame.width, frame.height, 0xFF000000 + 0x000004*i, i, i,
            (int*)frame.pixels);
        int frameSize = 0;
        revError = Revel_EncodeFrame(encoderHandle, &frame, &frameSize);
    	if (revError != REVEL_ERR_NONE)
        {
	        printf("Revel Error while writing frame: %d\n", revError);
	        exit(1);
	    }
        printf("Frame %d of %d: %d bytes\n", i+1, numFrames, frameSize);
    }

    // Encode the audio track.  NOTE that each call to Revel_EncodeAudio()
    // *appends* the new audio data onto the existing audio track.  There is
    // no synchronization between the audio and video tracks!  If you want
    // the audio to start on frame 60, you need to manually insert 60 frames
    // worth of silence at the beginning of your audio track!
    //
    // To demonstrate this, we'll encode the audio buffer twice. Note that
    // the two chimes play immediately when the movie starts, one after the
    // other, even though we're encoding them "after" all the video frames.
    int totalAudioBytes = 0;
    revError = Revel_EncodeAudio(encoderHandle, audioBuffer, audioBufferSize,
        &totalAudioBytes);
    revError = Revel_EncodeAudio(encoderHandle, audioBuffer, audioBufferSize,
        &totalAudioBytes);
    if (revError != REVEL_ERR_NONE)
    {
        printf("Revel Error while writing audio: %d\n", revError);
        exit(1);
    }
    printf("Encoded %d bytes of audio\n", totalAudioBytes);
    

    // Finalize encoding.  If this step is skipped, the output movie will be
    // unviewable!
    int totalSize = 0;
    revError = Revel_EncodeEnd(encoderHandle, &totalSize);
    if (revError != REVEL_ERR_NONE)
    {
	    printf("Revel Error while ending encoding: %d\n", revError);
	    exit(1);
    }
    printf("%s written: %dx%d, %d frames, %d bytes\n", filename, width, height,
        numFrames, totalSize);

    // Final cleanup.
    Revel_DestroyEncoder(encoderHandle);
    if (audioBuffer != NULL)
        delete [] audioBuffer;
    delete [] (int*)frame.pixels;
    return 0;
}
