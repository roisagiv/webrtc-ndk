/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "encoded_frame.h"
#include "generic_encoder.h"
#include "jitter_buffer_common.h"
#include "video_coding_defines.h"

namespace webrtc {

VCMEncodedFrame::VCMEncodedFrame()
:
webrtc::EncodedImage(),
_renderTimeMs(-1),
_payloadType(0),
_missingFrame(false),
_codecSpecificInfo(NULL),
_codecSpecificInfoLength(0),
_codec(kVideoCodecUnknown)
{
}

VCMEncodedFrame::VCMEncodedFrame(const webrtc::EncodedImage& rhs)
:
webrtc::EncodedImage(rhs),
_renderTimeMs(-1),
_payloadType(0),
_missingFrame(false),
_codecSpecificInfo(NULL),
_codecSpecificInfoLength(0),
_codec(kVideoCodecUnknown)
{
    _buffer = NULL;
    _size = NULL;
    _length = NULL;
    if (rhs._buffer != NULL)
    {
        VerifyAndAllocate(rhs._length);
        memcpy(_buffer, rhs._buffer, rhs._length);
    }
}

VCMEncodedFrame::VCMEncodedFrame(const VCMEncodedFrame& rhs)
:
webrtc::EncodedImage(rhs),
_renderTimeMs(rhs._renderTimeMs),
_payloadType(rhs._payloadType),
_missingFrame(rhs._missingFrame),
_codecSpecificInfo(NULL),
_codecSpecificInfoLength(0),
_codec(rhs._codec)
{
    _buffer = NULL;
    _size = NULL;
    _length = NULL;
    if (rhs._buffer != NULL)
    {
        VerifyAndAllocate(rhs._size);
        memcpy(_buffer, rhs._buffer, rhs._length);
    }
}

VCMEncodedFrame::~VCMEncodedFrame()
{
    Free();
}

void VCMEncodedFrame::Free()
{
    Reset();
    if (_buffer != NULL)
    {
        delete [] _buffer;
        _buffer = NULL;
    }
}

void VCMEncodedFrame::Reset()
{
    _renderTimeMs = -1;
    _timeStamp = 0;
    _payloadType = 0;
    _codecSpecificInfo = NULL;
    _codecSpecificInfoLength = 0;
    _frameType = kDeltaFrame;
    _encodedWidth = 0;
    _encodedHeight = 0;
    _completeFrame = false;
    _missingFrame = false;
    _length = 0;
    _codec = kVideoCodecUnknown;
}

WebRtc_Word32
VCMEncodedFrame::Store(VCMFrameStorageCallback& storeCallback) const
{
    EncodedVideoData frameToStore;
    frameToStore.codec = _codec;
    if (_buffer != NULL)
    {
        frameToStore.VerifyAndAllocate(_length);
        memcpy(frameToStore.payloadData, _buffer, _length);
        frameToStore.payloadSize = _length;
    }
    frameToStore.completeFrame = _completeFrame;
    frameToStore.encodedWidth = _encodedWidth;
    frameToStore.encodedHeight = _encodedHeight;
    frameToStore.frameType = ConvertFrameType(_frameType);
    frameToStore.missingFrame = _missingFrame;
    frameToStore.payloadType = _payloadType;
    frameToStore.renderTimeMs = _renderTimeMs;
    frameToStore.timeStamp = _timeStamp;
    storeCallback.StoreReceivedFrame(frameToStore);
    return VCM_OK;
}

WebRtc_Word32
VCMEncodedFrame::VerifyAndAllocate(const WebRtc_UWord32 minimumSize)
{
    if(minimumSize > _size)
    {
        // create buffer of sufficient size
        WebRtc_UWord8* newBuffer = new WebRtc_UWord8[minimumSize];
        if (newBuffer == NULL)
        {
            return -1;
        }
        if(_buffer)
        {
            // copy old data
            memcpy(newBuffer, _buffer, _size);
            delete [] _buffer;
        }
        _buffer = newBuffer;
        _size = minimumSize;
    }
    return 0;
}

webrtc::FrameType VCMEncodedFrame::ConvertFrameType(VideoFrameType frameType)
{
    switch(frameType)
    {
    case kKeyFrame:
        {
            return  kVideoFrameKey;
        }
    case kDeltaFrame:
        {
            return kVideoFrameDelta;
        }
    case kGoldenFrame:
        {
            return kVideoFrameGolden;
        }
    case kAltRefFrame:
        {
            return kVideoFrameAltRef;
        }
    default:
        {
            return kVideoFrameDelta;
        }
    }
}

VideoFrameType VCMEncodedFrame::ConvertFrameType(webrtc::FrameType frameType)
{
    switch (frameType)
    {
    case kVideoFrameKey:
        {
            return kKeyFrame;
        }
    case kVideoFrameDelta:
        {
            return kDeltaFrame;
        }
    case kVideoFrameGolden:
        {
            return kGoldenFrame;
        }
    case kVideoFrameAltRef:
        {
            return kAltRefFrame;
        }
    default:
        {
            return kDeltaFrame;
        }
    }
}

}
