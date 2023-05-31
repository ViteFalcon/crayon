#pragma once

#include <boost/iostreams/stream_buffer.hpp>
#include <vector>

#include "../core/string.h"
#include "server_request.h"

namespace crayon {

class Connection;

class ByteBufferWriter {
 public:
  typedef char char_type;
  typedef boost::iostreams::sink_tag category;

  ByteBufferWriter &write_u8(std::uint8_t value);

  ByteBufferWriter &write_u16(std::uint16_t value);

  ByteBufferWriter &write_u32(std::uint32_t value);

  ByteBufferWriter &write_u64(std::uint64_t value);

  template <size_t N>
  ByteBufferWriter &write_strl(const FixedBytesString<N> &value) {
    return write(value.ptr(), N);
  }

  ByteBufferWriter &write_strz(const String &value);

  ByteBufferWriter &write_nstr(const String &value);

  ByteBufferWriter &write(const void *data, size_t size);

  ByteBufferWriter &append(ByteBufferWriter &other);

  [[nodiscard]] inline bool empty() const { return _container.empty(); }

  [[nodiscard]] inline std::size_t size() const { return _container.size(); }

  [[nodiscard]] const std::vector<char> &bytes() const { return _container; }

 private:
  std::vector<char> _container;
};

class NetworkPacketBuilder {
 public:
  NetworkPacketBuilder(ServerRequest request, Connection &connection);
  ~NetworkPacketBuilder();

  inline NetworkPacketBuilder &write_u8(std::uint8_t value) {
    _writer.write_u8(value);
    return *this;
  }

  inline NetworkPacketBuilder &write_u16(std::uint16_t value) {
    _writer.write_u16(value);
    return *this;
  }

  inline NetworkPacketBuilder &write_u32(std::uint32_t value) {
    _writer.write_u32(value);
    return *this;
  }

  inline NetworkPacketBuilder &write_u64(std::uint64_t value) {
    _writer.write_u64(value);
    return *this;
  }

  template <size_t N>
  inline NetworkPacketBuilder &write_strl(const FixedBytesString<N> &value) {
    _writer.write_strl(value);
    return *this;
  }

  inline NetworkPacketBuilder &write_strz(const String &value) {
    _writer.write_strz(value);
    return *this;
  }

  inline NetworkPacketBuilder &write_nstr(const String &value) {
    _writer.write_nstr(value);
    return *this;
  }

  NetworkPacketBuilder &write(const void *data, size_t size) {
    _writer.write(data, size);
    return *this;
  }

  void send();

 private:
  ByteBufferWriter _writer;
  Connection &_connection;
};
}  // namespace crayon