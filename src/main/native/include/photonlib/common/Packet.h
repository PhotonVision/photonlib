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

#include <wpi/Endian.h>

#include <algorithm>
#include <string>
#include <vector>

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
    m_isValid = true;
  }

  /* Get a pointer to the data contained in the packet
   *
   * Warning: the returned pointer may become invalid after you append data
   * to the packet, therefore it should never be stored.
   *
   * The return pointer is nullptr if the packet is empty.
   */
  const void* getData() const {
    return !m_packetData.empty() ? &m_packetData[0] : nullptr;
  }

  /* Get the size of the data contained in the packet in bytes
   *
   * This function returns the number of bytes pointed to by what getData
   * returns.
   */
  size_t getDataSize() const;

 public:
  template <typename T>
  Packet& operator<<(T src) {
    std::memcpy(m_packetData.data() + m_readPos, &src, sizeof(T));

    if constexpr (wpi::support::endian::system_endianness() ==
                  wpi::support::endianness::little) {
      // Reverse to big endian for network conventions.
      std::reverse(m_packetData.data() + m_readPos,
                   m_packetData.data() + m_readPos + sizeof(T));
    }

    m_readPos += sizeof(T);
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

  // Reading state of the packet
  bool m_isValid = true;

  // Current reading position in the packet
  size_t m_readPos = 0;

  bool operator==(const Packet& right) const {
    return m_packetData == right.m_packetData;
  }
  bool operator!=(const Packet& right) const { return !operator==(right); }
};

}  // namespace photonlib
