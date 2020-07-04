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
#include <cstring>
#include <vector>

namespace photonlib {
/**
 * Represents a byte packable message that is sent over NetworkTables.
 */
class BytePackable {
 public:
  /**
   * Converts this object to a serialized byte array.
   */
  virtual std::vector<char> ToByteArray() = 0;

  /**
   * Deserializes the byte array into the current object.
   */
  virtual void FromByteArray(std::vector<char> src) = 0;

  /**
   * Get the current buffer position.
   */
  int GetBufferPosition() const { return bufferPosition; }

  /**
   * Resets the buffer position to zero.
   */
  void ResetBufferPosition() { bufferPosition = 0; }

 protected:
  int bufferPosition = 0;
  /**
   * Converts a source to bytes and copies it into a destination byte array at
   * the index of the buffer position.
   *
   * @param src The source to convert to bytes.
   * @param dest The destination byte array.
   */
  template <typename T>
  void BufferData(T src, std::vector<char>* dest) {
    std::memcpy((*dest).data() + bufferPosition, &src, sizeof(T));

    if constexpr (wpi::support::endian::system_endianness() ==
                  wpi::support::endianness::little) {
      // Reverse to big endian for network conventions.
      std::reverse((*dest).data() + bufferPosition,
                   (*dest).data() + bufferPosition + sizeof(T));
    }

    bufferPosition += sizeof(T);
  }

  /**
   * Returns an object (of a specified type) that is extracted from the provided
   * source byte array at the index of the buffer position.
   *
   * @param src The source byte array.
   * @return The extracted object.
   */
  template <typename T>
  T UnbufferData(const std::vector<char>& src) {
    T value;
    char bytes[sizeof(T)];
    std::memcpy(&bytes, src.data() + bufferPosition, sizeof(T));

    if constexpr (wpi::support::endian::system_endianness() ==
                  wpi::support::endianness::little) {
      // Reverse to little endian for host.
      std::reverse(&bytes[0], &bytes[sizeof(T)]);
    }

    std::memcpy(&value, &bytes, sizeof(T));
    bufferPosition += sizeof(T);
    return value;
  }
};
}  // namespace photonlib
