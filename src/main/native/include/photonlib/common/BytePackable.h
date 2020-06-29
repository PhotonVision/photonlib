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

#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

namespace photonlib {

class BytePackable {
 public:
  virtual std::string ToByteArray() = 0;
  virtual void FromByteArray(std::string src) = 0;
  int GetBufferPosition() const { return bufferPosition; }
  void ResetBufferPosition() { bufferPosition = 0; }

 protected:
  int bufferPosition = 0;

  void BufferData(std::string src, std::string* dest) {
    (*dest).assign(src, bufferPosition, src.length());
    bufferPosition += src.length();
  }

  void BufferData(char src, std::string* dest) {
    BufferData(std::string(1, src), dest);
  }

  void BufferData(int src, std::string* dest) {
    BufferData(std::to_string(src), dest);
  }

  void BufferData(double src, std::string* dest) {
    BufferData(std::to_string(src), dest);
  }

  void BufferData(bool src, std::string* dest) {
    BufferData(std::string(src ? "1" : "0"), dest);
  }

  char UnbufferByte(std::string src) {
    auto& value = src.at(bufferPosition);
    bufferPosition++;
    return value;
  }

  std::string UnbufferBytes(std::string src, int len) {
    auto value = src.substr(bufferPosition, bufferPosition + len);
    bufferPosition += len;
    return value;
  }

  int UnbufferInt(std::string src) {
    int value;
    std::memcpy(&value, &(src.at(bufferPosition)), sizeof(int));
    bufferPosition += sizeof(int);
    return value;
  }

  double UnbufferDouble(std::string src) {
    double value;
    std::memcpy(&value, &(src.at(bufferPosition)), sizeof(double));
    bufferPosition += sizeof(double);
    return value;
  }

  bool UnbufferBoolean(std::string src) {
    bool value = src.at(bufferPosition) != 0;
    bufferPosition++;
    return value;
  }
};

}  // namespace photonlib
