/*
Copyright (C) (2004) (Cort Stratton) <cort at cortstratton dot org>

This program is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License 
as published by the Free Software Foundation; either 
version 2 of the License, or (at your option) any later 
version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "revel.h"
#include "BaseEncoder.h"

#include "XvidEncoder.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace
{
    Revel_Error EncoderFactory(const Revel_Params *params, Revel_BaseEncoder **encoderOut)
    {
        Revel_BaseEncoder *enc = NULL;
        
        switch(params->codec)
        {
        case REVEL_CD_XVID:
            enc = new Revel_XvidEncoder(*params);
            break;
        default:
            return REVEL_ERR_PARAMS;
        }

        if (enc == NULL)
            return REVEL_ERR_MEMORY;
        *encoderOut = enc;
        return REVEL_ERR_NONE;
    }

	// Giant wrapper class whose sole purpose is to make sure it's safe to call Revel functions
	// from global static initialization code.
	class EncoderMgr
	{
	public:
		EncoderMgr()
		{
			atexit(_Destroy);
		}

		static Revel_Error CreateEncoder(int *outHandle, const Revel_Params *params)
		{
			// must access the array through this function, to ensure that it gets allocated.
			Revel_BaseEncoder **encoders = _GetEncoderArray();
			assert(encoders != NULL);
			// If we've filled the current block of 256 handles, we need to allocate more.
			if (s_nextHandle % 256 == 0)
			{
				Revel_BaseEncoder **oldPtr = s_encoders;
				// allocate up to the next multiple of 255
				s_encoders = (Revel_BaseEncoder**)realloc(oldPtr, ((s_nextHandle+0xFF) & ~0xFF) * sizeof(Revel_BaseEncoder*));
				if (s_encoders == NULL)
				{
					s_encoders = oldPtr;
					return REVEL_ERR_MEMORY;
				}
			}

			encoders[s_nextHandle] = NULL;
			Revel_BaseEncoder *newEnc = NULL;
			Revel_Error revError = EncoderFactory(params, &newEnc);
			if (revError != REVEL_ERR_NONE)
			{
				*outHandle = 0;
				return revError;
			}
			encoders[s_nextHandle] = newEnc;
			*outHandle = s_nextHandle++;
			return REVEL_ERR_NONE;
		}

		static Revel_BaseEncoder *GetEncoder(int handle)
		{
			// No error checking; callers are responsible for calling IsHandleValid()
			Revel_BaseEncoder **encoders = _GetEncoderArray();
			return encoders[handle];
		}

		static bool IsEncoderValid(int handle)
		{
			Revel_BaseEncoder **encoders = _GetEncoderArray();
			assert(encoders != NULL);
			if (handle == 0 || handle >= s_nextHandle)
				return false;
			return (encoders[handle] != NULL);
		}

		static Revel_Error DestroyEncoder(int handle)
		{
			// deleting invalid handles is a no-op.
			if (!IsEncoderValid(handle))
				return REVEL_ERR_NONE;

			// Delete the encoder, if there is one.
			Revel_BaseEncoder **encoders = _GetEncoderArray();
			delete encoders[handle];
			encoders[handle] = NULL;

			return REVEL_ERR_NONE;
		}
	private:
		static Revel_BaseEncoder **s_encoders;
		static int s_nextHandle;

		//! Access the singleton instance
		static inline Revel_BaseEncoder **_GetEncoderArray(void)
		{
			if (s_encoders == NULL)
			{
				s_encoders = (Revel_BaseEncoder**)malloc( ((s_nextHandle+0xFF) & ~0xFF) * sizeof(Revel_BaseEncoder*) );
			}
			return s_encoders;
		}

		//! Destroys singleton instance
		static void _Destroy(void)
		{
			for(int iHandle=0; iHandle<s_nextHandle; ++iHandle)
			{
				DestroyEncoder(iHandle);
			}
			free(s_encoders);
			s_encoders = NULL;
		}
	};
	Revel_BaseEncoder **EncoderMgr::s_encoders = NULL;
	int EncoderMgr::s_nextHandle = 1;
}

int Revel_GetVersion(void)
{
    return REVEL_VERSION;
}
int Revel_GetApiVersion(void)
{
    return REVEL_API_VERSION;
}


Revel_Error Revel_InitializeParams(Revel_Params *params)
{
    if (params == NULL)
        return REVEL_ERR_PARAMS;
    params->width = params->height = 1;
    params->hasAudio = 0;
    params->audioBits = 16;
    params->audioChannels = 2;
    params->audioRate = 44100;
    params->audioSampleFormat = REVEL_ASF_UNKNOWN;
    params->codec = REVEL_CD_XVID;
	params->origin = REVEL_ORIGIN_UPPER_LEFT;
    params->frameRate = 30;
    params->quality = 1.0f;
	return REVEL_ERR_NONE;
}


Revel_Error Revel_CreateEncoder(int *encoderHandle, const Revel_Params *params)
{
    if (encoderHandle == NULL || params == NULL)
        return REVEL_ERR_PARAMS;

	return EncoderMgr::CreateEncoder(encoderHandle, params);
}


bool Revel_IsEncoderValid(int encoderHandle)
{
	return EncoderMgr::IsEncoderValid(encoderHandle);
}


Revel_Error Revel_EncodeStart(int encoderHandle, const char* filename)
{
    if (filename == NULL)
        return REVEL_ERR_PARAMS;
    if (!Revel_IsEncoderValid(encoderHandle))
        return REVEL_ERR_INVALID_HANDLE;

	return EncoderMgr::GetEncoder(encoderHandle)->EncodeStart(filename);
}


Revel_Error Revel_EncodeFrame(int encoderHandle,
                              Revel_VideoFrame *frame, int *frameSize)
{
    if (frame == NULL)
        return REVEL_ERR_PARAMS;
    if (!Revel_IsEncoderValid(encoderHandle))
        return REVEL_ERR_INVALID_HANDLE;
    return EncoderMgr::GetEncoder(encoderHandle)->EncodeFrame(*frame, frameSize);
}


Revel_Error Revel_EncodeAudio(int encoderHandle, void *sampleBuffer,
                              int numBytes, int *numTotalBytes)
{
    if (!Revel_IsEncoderValid(encoderHandle))
        return REVEL_ERR_INVALID_HANDLE;
    return EncoderMgr::GetEncoder(encoderHandle)->EncodeAudio(sampleBuffer, numBytes,
        numTotalBytes);
}


Revel_Error Revel_EncodeEnd(int encoderHandle, int *totalSize)
{
    if (!Revel_IsEncoderValid(encoderHandle))
        return REVEL_ERR_INVALID_HANDLE;
    return EncoderMgr::GetEncoder(encoderHandle)->EncodeEnd(totalSize);
}



Revel_Error Revel_DestroyEncoder(int encoderHandle)
{
	return EncoderMgr::DestroyEncoder(encoderHandle);
}
