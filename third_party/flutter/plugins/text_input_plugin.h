// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_GLFW_TEXT_INPUT_PLUGIN_H_
#define FLUTTER_SHELL_PLATFORM_GLFW_TEXT_INPUT_PLUGIN_H_

#include <map>
#include <memory>

#include "../binary_messenger.h"
#include "../method_channel.h"
#include "../text_input_model.h"
#include "keyboard_hook_handler.h"

#include "rapidjson/document.h"

class EglWindow;

namespace flutter {

// Implements a text input plugin.
//
// Specifically handles window events within GLFW.
class TextInputPlugin : public KeyboardHookHandler {
 public:
  explicit TextInputPlugin(flutter::BinaryMessenger* messenger);

  virtual ~TextInputPlugin();

  // |KeyboardHookHandler|
  void KeyboardHook(EglWindow* window,
                    int key,
                    int scancode,
                    int action,
                    int mods) override;

  // |KeyboardHookHandler|
  void CharHook(EglWindow* window, unsigned int code_point) override;

 private:
  // Sends the current state of the given model to the Flutter engine.
  void SendStateUpdate(const TextInputModel& model);

  // Sends an action triggered by the Enter key to the Flutter engine.
  void EnterPressed(TextInputModel* model);

  // Called when a method is called on |channel_|;
  void HandleMethodCall(
      const flutter::MethodCall<rapidjson::Document>& method_call,
      std::unique_ptr<flutter::MethodResult<rapidjson::Document>> result);

  // The MethodChannel used for communication with the Flutter engine.
  std::unique_ptr<flutter::MethodChannel<rapidjson::Document>> channel_;

  // The active client id.
  int client_id_;

  // The active model. nullptr if not set.
  std::unique_ptr<TextInputModel> active_model_;

  // Keyboard type of the client. See available options:
  // https://docs.flutter.io/flutter/services/TextInputType-class.html
  std::string input_type_;

  // An action requested by the user on the input client. See available options:
  // https://docs.flutter.io/flutter/services/TextInputAction-class.html
  std::string input_action_;
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_GLFW_TEXT_INPUT_PLUGIN_H_
