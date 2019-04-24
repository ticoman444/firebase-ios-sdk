/*
 * Copyright 2019 Google
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

#include "Firestore/core/src/firebase/firestore/objc/objc_class.h"

namespace firebase {
namespace firestore {
namespace objc {

HandleBase::HandleBase() : object_(nil) {
}

HandleBase::HandleBase(id object) : object_(object) {
}

HandleBase::HandleBase(const HandleBase& other)
    : object_(other.object_) {
}

HandleBase::HandleBase(HandleBase&& other) noexcept
    : object_(nil) {
  using std::swap;
  swap(object_, other.object_);
}


HandleBase::~HandleBase() {
  Release();
}

HandleBase& HandleBase::operator=(const HandleBase& other) {
  object_ = other.object_;
  return *this;
}

HandleBase& HandleBase::operator=(HandleBase&& other) noexcept {
  using std::swap;
  swap(object_, other.object_);
  return *this;
}


void HandleBase::Assign(id object) {
  object_ = object;
}

void HandleBase::Release() {
  object_ = nil;
}

}  // namespace objc
}  // namespace firestore
}  // namespace firebase
