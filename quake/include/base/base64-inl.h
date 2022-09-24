#ifndef _BASE_BASE64_INL_H_
#define _BASE_BASE64_INL_H_

namespace base {

inline std::string Base64Encode(const std::uint8_t* in_data,
                                std::size_t in_size, Base64EncodePolicy p) {
  std::string o;
  Base64Encode(in_data, in_size, &o, p);
  return o;
}

inline void Base64Encode(const std::string& input, std::string* output,
                         Base64EncodePolicy p) {
  Base64Encode(reinterpret_cast<const std::uint8_t*>(input.data()),
               input.size(), output, p);
}

inline std::string Base64Encode(const std::string& input,
                                Base64EncodePolicy p) {
  return Base64Encode(reinterpret_cast<const std::uint8_t*>(input.data()),
                      input.size(), p);
}

inline bool Base64Decode(const std::string& input, std::string* output,
                         Base64CharMatchFunction match, Base64DecodePolicy p) {
  return Base64Decode(reinterpret_cast<const std::uint8_t*>(input.data()),
                      input.size(), output, match, p);
}

inline std::size_t Base64EncodeSize(std::size_t n) { return 4 * ((n + 2) / 3); }

inline std::size_t Base64DecodeSize(std::size_t n) {
  const std::size_t r = n / 4 * 3;
  if (r) {
    return r;
  } else {
    DCHECK(n <= 3);
    return (n - 1);
  }
}

}  // namespace base

#endif  // _BASE_BASE64_INL_H_
