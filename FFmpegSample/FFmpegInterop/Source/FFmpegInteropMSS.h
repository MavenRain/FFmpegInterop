﻿//*****************************************************************************
//
//	Copyright 2015 Microsoft Corporation
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
//
//	http ://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
//*****************************************************************************

#pragma once
#include <queue>
#include "MediaSampleProvider.h"
#include "FFmpegReader.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Media::Core;

extern "C"
{
#include <libavformat/avformat.h>
}

namespace FFmpegInterop
{
	public ref class FFmpegInteropMSS sealed
	{
	public:
		static FFmpegInteropMSS^ CreateFFmpegInteropMSSFromStream(IRandomAccessStream^ stream, bool forceAudioDecode, bool forceVideoDecode);
		static FFmpegInteropMSS^ CreateFFmpegInteropMSSFromUri(String^ uri, bool forceAudioDecode, bool forceVideoDecode);

		// Contructor
		MediaStreamSource^ GetMediaStreamSource();
		virtual ~FFmpegInteropMSS();

	internal:
		int ReadPacket();

	private:
		FFmpegInteropMSS();

		HRESULT CreateMediaStreamSource(IRandomAccessStream^ stream, bool forceAudioDecode, bool forceVideoDecode);
		HRESULT CreateMediaStreamSource(String^ uri, bool forceAudioDecode, bool forceVideoDecode);
		HRESULT InitFFmpegContext(bool forceAudioDecode, bool forceVideoDecode);
		HRESULT CreateAudioStreamDescriptor(bool forceAudioDecode);
		HRESULT CreateVideoStreamDescriptor(bool forceVideoDecode);
		void OnStarting(MediaStreamSource ^sender, MediaStreamSourceStartingEventArgs ^args);
		void OnSampleRequested(MediaStreamSource ^sender, MediaStreamSourceSampleRequestedEventArgs ^args);

		MediaStreamSource^ mss;
		EventRegistrationToken startingRequestedToken;
		EventRegistrationToken sampleRequestedToken;

		internal:
		AVIOContext* avIOCtx;
		AVFormatContext* avFormatCtx;
		AVCodecContext* avAudioCodecCtx;
		AVCodecContext* avVideoCodecCtx;

		private:
		AudioStreamDescriptor^ audioStreamDescriptor;
		VideoStreamDescriptor^ videoStreamDescriptor;
		int audioStreamIndex;
		int videoStreamIndex;

		MediaSampleProvider^ audioSampleProvider;
		MediaSampleProvider^ videoSampleProvider;

		TimeSpan mediaDuration;
		IStream* fileStreamData;
		unsigned char* fileStreamBuffer;
		FFmpegReader^ m_pReader;
	};
}
