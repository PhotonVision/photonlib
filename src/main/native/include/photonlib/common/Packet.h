/**
 * Copyright (C) 2020 Photon Vision.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include <wpi/Endian.h>

namespace photonlib {

/**
 * Utility class to build blocks of data to transfer over the network
 */
class Packet {
 public:
  Packet() = default;
  explicit Packet(std::vector<char> data) : m_packetData(data) {}

  // Empty the packet
  void clear() {
    m_packetData.clear();
    m_readPos = 0;
  }

  const std::vector<char> getData() { return m_packetData; }

  /* Get the size of the data contained in the packet in bytes
   *
   * This function returns the number of bytes pointed to by what getData
   * returns.
   */
  size_t getDataSize() const { return m_packetData.size(); }

  template <typename T>
  Packet& operator<<(T src) {
    m_packetData.resize(m_packetData.size() + sizeof(T));
    std::memcpy(m_packetData.data() + m_writePos, &src, sizeof(T));

    if constexpr (wpi::support::endian::system_endianness() ==
                  wpi::support::endianness::little) {
      // Reverse to big endian for network conventions.
      std::reverse(m_packetData.data() + m_writePos,
                   m_packetData.data() + m_writePos + sizeof(T));
    }

    m_writePos += sizeof(T);
    return *this;
  }

  template <typename T>
  Packet& operator>>(T& value) {
    char bytes[sizeof(T)];
    std::memcpy(&bytes, m_packetData.data() + m_readPos, sizeof(T));

    if constexpr (wpi::support::endian::system_endianness() ==
                  wpi::support::endianness::little) {
      // Reverse to little endian for host.
      std::reverse(&bytes[0], &bytes[sizeof(T)]);
    }

    std::memcpy(&value, &bytes, sizeof(T));
    m_readPos += sizeof(T);
    return *this;
  }

 private:
  // Data stored in the packet
  std::vector<char> m_packetData;

  size_t m_readPos = 0;
  size_t m_writePos = 0;

  bool operator==(const Packet& right) const {
    return m_packetData == right.m_packetData;
  }
  bool operator!=(const Packet& right) const { return !operator==(right); }
};

}  // namespace photonlib
