#ifndef _BASE_BASE64_H_
#define _BASE_BASE64_H_
#include "check.h"
#include "common.h"

// The following base64 encoding/decoding code is taken from the |Chrome| source
// The reason is because we need to implement the legacy atob/btoa Web api
// which does specific logic. The code in |Chrome| is also originated from the
// |WebKit| project
//  License is reserved as following for future reference
// ---------------------------------------------------------------------------
/*
 * Copyright (C) 2006 Alexey Proskuryakov <ap@webkit.org>
 * Copyright (C) 2010 Patrick Gansterer <paroga@paroga.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ---------------------------------------------------------------------------

namespace base {

enum class Base64EncodePolicy { kBase64DoNotInsertLFs, kBase64InsertLFs };

enum class Base64DecodePolicy {
  kBase64DoNotValidatePadding,
  kBase64ValidatePadding
};

using Base64CharMatchFunction = bool (*)(std::uint8_t);

void Base64Encode(
    const std::uint8_t*, std::size_t, std::string*,
    Base64EncodePolicy = Base64EncodePolicy::kBase64DoNotInsertLFs);

inline std::string Base64Encode(
    const std::uint8_t* in_data, std::size_t in_size,
    Base64EncodePolicy p = Base64EncodePolicy::kBase64DoNotInsertLFs);

inline void Base64Encode(
    const std::string& input, std::string* output,
    Base64EncodePolicy p = Base64EncodePolicy::kBase64DoNotInsertLFs);

inline std::string Base64Encode(
    const std::string& input,
    Base64EncodePolicy p = Base64EncodePolicy::kBase64DoNotInsertLFs);

bool Base64Decode(
    const std::uint8_t*, std::size_t, std::string*,
    Base64CharMatchFunction match = nullptr,
    Base64DecodePolicy = Base64DecodePolicy::kBase64DoNotValidatePadding);

inline bool Base64Decode(
    const std::string& input, std::string* output,
    Base64CharMatchFunction match = nullptr,
    Base64DecodePolicy p = Base64DecodePolicy::kBase64DoNotValidatePadding);

inline std::size_t Base64EncodeSize(std::size_t n);
inline std::size_t Base64DecodeSize(std::size_t n);

// Special function for special usage, ie performance etc ...

// Similar as base64encode, but just do the encoding as appending to existed
// std::string
void Base64EncodeAppend(
    const std::string&, std::string*,
    Base64EncodePolicy = Base64EncodePolicy::kBase64DoNotInsertLFs);

}  // namespace base

#include "base64-inl.h"

#endif  // _BASE_BASE64_H_
