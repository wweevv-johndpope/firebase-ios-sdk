/*
 * Copyright 2018 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Firestore/core/src/util/string_win.h"

#if defined(_WIN32)
#include <windows.h>

#include "Firestore/core/src/util/hard_assert.h"
#include "absl/memory/memory.h"
#include "absl/strings/match.h"

namespace firebase {
namespace firestore {
namespace util {

std::wstring Utf8ToNative(absl::string_view input) {
  int input_len = static_cast<int>(input.size());

  // MultiByteToWideChar considers a zero length to be an error, so special case
  // the empty string.
  if (input_len == 0) {
    return L"";
  }

  // The input string_view may contain embedded nulls and isn't necessarily null
  // terminated so we must pass an explicit length to MultiByteToWideChar. This
  // means the result is the number of wchar_t required to hold the result,
  // excluding the null terminator.
  int output_len =
      ::MultiByteToWideChar(CP_UTF8, 0, input.data(), input_len, nullptr, 0);
  if (output_len == 0) {
    DWORD error = ::GetLastError();
    HARD_FAIL("Utf8ToNative failed with code %s: %s", error,
              LastErrorMessage(error));
  }
  HARD_ASSERT(output_len > 0);

  int output_terminated_len = output_len + 1;
  std::wstring output(output_terminated_len, L'\0');
  int result = ::MultiByteToWideChar(CP_UTF8, 0, input.data(), input_len,
                                     &output[0], output_len);
  HARD_ASSERT(result > 0 && result < output_terminated_len &&
              output[output_len] == '\0');

  output.resize(result);
  return output;
}

std::string NativeToUtf8(const std::wstring& input) {
  return NativeToUtf8(input.c_str(), input.size());
}

std::wstring SysNativeMBToWide(const wchar_t* const input, const size_t input_size) {
  mbstate_t ps;

  // Calculate the number of wide characters.  We walk through the string
  // without writing the output, counting the number of wide characters.
  size_t num_out_chars = 0;
  memset(&ps, 0, sizeof(ps));
  for (size_t i = 0; i < input_size; ) {
    const char* src = input + i;
    size_t res = mbrtowc(nullptr, src, input_size - i, &ps);
    switch (res) {
      // Handle any errors and return an empty string.
      case static_cast<size_t>(-2):
      case static_cast<size_t>(-1):
        return std::wstring();
      case 0:
        // We hit an embedded null byte, keep going.
        i += 1;
      default:
        i += res;
        ++num_out_chars;
        break;
    }
  }

  if (num_out_chars == 0)
    return std::wstring();

  std::wstring out;
  out.resize(num_out_chars);

  memset(&ps, 0, sizeof(ps));  // Clear the shift state.
  // We walk the input string again, with |i| tracking the index of the
  // multi-byte input, and |j| tracking the wide output.
  for (size_t i = 0, j = 0; i < input_size; ++j) {
    const char* src = input + i;
    wchar_t* dst = &out[j];
    size_t res = mbrtowc(dst, src, input_size - i, &ps);
    switch (res) {
      // Handle any errors and return an empty string.
      case static_cast<size_t>(-2):
      case static_cast<size_t>(-1):
        return std::wstring();
      case 0:
        i += 1;  // Skip null byte.
        break;
      default:
        i += res;
        break;
    }
  }

  return out;
}

std::string NativeToUtf8(const wchar_t* input, size_t input_size) {
  const std::wstring wide_input = SysNativeMBToWide(input, input_size);
  input = wide_input.c_str();
  input_size = wide_input.size();

  int input_len = static_cast<int>(input_size);
  if (input_len == 0) {
    return "";
  }

  // The input buffer may contain embedded nulls and isn't necessarily null
  // terminated so we must pass an explicit length to WideCharToMultiByte. This
  // means the result is the number of char required to hold the result,
  // excluding the null terminator.
  int output_len = ::WideCharToMultiByte(CP_UTF8, 0, input, input_len, nullptr,
                                         0, nullptr, nullptr);
  if (output_len == 0) {
    DWORD error = ::GetLastError();
    HARD_FAIL("NativeToUtf8 failed with code %s: %s", error,
              LastErrorMessage(error));
  }
  HARD_ASSERT(output_len > 0);

  int output_terminated_len = output_len + 1;
  std::string output(output_terminated_len, '\0');
  int result = ::WideCharToMultiByte(CP_UTF8, 0, input, input_len, &output[0],
                                     output_len, nullptr, nullptr);
  HARD_ASSERT(result > 0 && result < output_terminated_len &&
              output[output_len] == '\0');

  output.resize(result);
  return output;
}

std::string LastErrorMessage(DWORD last_error) {
  // Preallocate a buffer sufficiently large to receive any message. Since we're
  // not asking for inserts this is already way too big.
  size_t size = 16 * 1024;
  auto error_text = absl::make_unique<wchar_t[]>(size);

  // output_len excludes the trailing null.
  DWORD output_len = ::FormatMessageW(
      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
      last_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_text.get(),
      static_cast<DWORD>(size), nullptr);
  if (output_len == 0) {
    DWORD format_error = ::GetLastError();
    return util::StringFormat(
        "error %s; unknown error %s while getting error text", last_error,
        format_error);
  }

  std::string formatted = NativeToUtf8(error_text.get(), output_len);
  if (absl::EndsWith(formatted, "\r\n")) {
    formatted.resize(formatted.size() - 2);
  }
  return formatted;
}

}  // namespace util
}  // namespace firestore
}  // namespace firebase

#endif  // defined(_WIN32)
