#include "network_packet_builder.h"

#include <boost/endian.hpp>

#include "connection.h"
#include "server_request.h"

namespace crayon {

ByteBufferWriter &ByteBufferWriter::write_u8(std::uint8_t value) {
  // log::debug("... adding 1 byte to the packet");
  _container.emplace_back(value);
  return *this;
}

ByteBufferWriter &ByteBufferWriter::write_u16(std::uint16_t value) {
  boost::endian::little_uint16_t le_value = value;
  return write(le_value.data(), sizeof(std::uint16_t));
}

ByteBufferWriter &ByteBufferWriter::write_u32(std::uint32_t value) {
  boost::endian::little_uint32_t le_value = value;
  return write(le_value.data(), sizeof(std::uint32_t));
}

ByteBufferWriter &ByteBufferWriter::write_u64(std::uint64_t value) {
  boost::endian::little_uint64_t le_value = value;
  return write(le_value.data(), sizeof(std::uint64_t));
}

ByteBufferWriter &ByteBufferWriter::write_strz(const String &value) {
  auto bytes = to_bytes(value);
  return write(&bytes.data[0], bytes.data.size()).write_u8(0);
}

ByteBufferWriter &ByteBufferWriter::write_nstr(const String &value) {
  auto bytes = to_bytes(value);
  return write_u16(bytes.data.size()).write(&bytes.data[0], bytes.data.size());
}

ByteBufferWriter &ByteBufferWriter::write(const void *data, size_t size) {
  const char *d = (const char *)data;
  // log::debug("... adding {} bytes to the packet", size);
  _container.insert(_container.end(), d, d + size);
  return *this;
}

ByteBufferWriter &ByteBufferWriter::append(crayon::ByteBufferWriter &other) {
  _container.insert(_container.end(), other._container.begin(), other._container.end());
  return *this;
}

NetworkPacketBuilder::NetworkPacketBuilder(ServerRequest request, Connection &connection) : _connection(connection) {
  write_u16((std::uint16_t)request);
}

NetworkPacketBuilder::~NetworkPacketBuilder() { send(); }

void NetworkPacketBuilder::send() {
  if (_writer.empty()) {
    // The data was committed to the connection
    return;
  }
  _connection.async_write(_writer.bytes());
  _writer = ByteBufferWriter();
}

}  // namespace crayon