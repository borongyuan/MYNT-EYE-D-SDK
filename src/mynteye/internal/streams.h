// Copyright 2018 Slightech Co., Ltd. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef MYNTEYE_INTERNAL_STREAMS_H_
#define MYNTEYE_INTERNAL_STREAMS_H_
#pragma once

#include <map>
#include <thread>
#include <vector>

#include "mynteye/data/types_internal.h"
#include "mynteye/internal/blocking_queue.h"
#include "mynteye/types.h"

MYNTEYE_BEGIN_NAMESPACE

class Device;

class Streams {
 public:
  using data_t = StreamData;
  using datas_t = std::vector<data_t>;
  using image_queue_t = BlockingQueue<Image::pointer>;
  using image_queue_ptr_t = std::shared_ptr<image_queue_t>;

  explicit Streams(std::shared_ptr<Device> device);
  ~Streams();

  /**
   * Enable image infos.
   *
   * If sync is false, indicates only can get infos from callback.
   * If sync is true, indicates can get infos from callback or access it from StreamData.
   */
  void EnableImageInfo(bool sync);
  bool IsImageInfoEnabled();

  void EnableStreamData(const ImageType& type);
  bool IsStreamDataEnabled(const ImageType& type);
  bool HasStreamDataEnabled();

  data_t GetStreamData(const ImageType& type);
  datas_t GetStreamDatas(const ImageType& type);

  void OnCameraOpen();
  void OnCameraClose();

  void OnImageInfoCallback(const ImgInfoPacket &packet);

 private:
  bool IsRightColorSupported();

  void StartImageCapturing();
  void StopImageCapturing();

  void OnColorCaptured(const Image::pointer& color);
  void OnLeftColorCaptured(const Image::pointer& color);
  void OnRightColorCaptured(const Image::pointer& color);
  void OnDepthCaptured(const Image::pointer& depth);

  void PushImage(const Image::pointer& color);

  std::shared_ptr<Device> device_;

  bool is_image_info_enabled_;
  bool is_image_info_sync_;

  bool is_right_color_supported_;

  // std::set<ImageType> image_enabled_set_;
  std::map<ImageType, image_queue_ptr_t> image_queue_map_;

  bool is_image_capturing_;
  std::thread image_capture_thread_;
};

MYNTEYE_END_NAMESPACE

#endif  // MYNTEYE_INTERNAL_STREAMS_H_