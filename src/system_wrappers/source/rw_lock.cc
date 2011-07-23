/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "rw_lock_wrapper.h"

#include <assert.h>

#if defined(_WIN32)
    #include "rw_lock_windows.h"
#elif defined(ANDROID)
    #include <stdlib.h>
    #include "rw_lock_generic.h"
#else
    #include "rw_lock_linux.h"
#endif

namespace webrtc {
RWLockWrapper* RWLockWrapper::CreateRWLock()
{
#ifdef _WIN32
    RWLockWrapper* lock =  new RWLockWindows();
#elif defined(ANDROID)
    RWLockWrapper* lock =  new RWLockWrapperGeneric();
#else
    RWLockWrapper* lock =  new RWLockLinux();
#endif
    if(lock->Init() != 0)
    {
        delete lock;
        assert(false);
        return NULL;
    }
    return lock;
}

RWLockWrapper::~RWLockWrapper()
{
}
} // namespace webrtc
