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

#ifndef FIRESTORE_CORE_SRC_FIREBASE_FIRESTORE_API_FIRESTORE_H_
#define FIRESTORE_CORE_SRC_FIREBASE_FIRESTORE_API_FIRESTORE_H_

#if !defined(__OBJC__)
#error "This header only supports Objective-C++"
#endif  // !defined(__OBJC__)

#import <Foundation/Foundation.h>

#include <memory>
#include <mutex>  // NOLINT(build/c++11)
#include <string>
#include <utility>
#include "dispatch/dispatch.h"

#include "Firestore/core/src/firebase/firestore/api/settings.h"
#include "Firestore/core/src/firebase/firestore/auth/credentials_provider.h"
#include "Firestore/core/src/firebase/firestore/core/transaction.h"
#include "Firestore/core/src/firebase/firestore/model/database_id.h"
#include "Firestore/core/src/firebase/firestore/util/async_queue.h"
#include "Firestore/core/src/firebase/firestore/util/hard_assert.h"
#include "Firestore/core/src/firebase/firestore/util/status.h"
#include "Firestore/core/src/firebase/firestore/util/statusor_callback.h"
#include "absl/types/any.h"

NS_ASSUME_NONNULL_BEGIN

@class FIRCollectionReference;
@class FIRQuery;
@class FIRTransaction;
@class FSTFirestoreClient;

namespace firebase {
namespace firestore {
namespace api {

class DocumentReference;
class WriteBatch;

class Firestore : public std::enable_shared_from_this<Firestore> {
 public:
  using TransactionBlock = id _Nullable (^)(FIRTransaction*, NSError** error);

  Firestore() = default;

  Firestore(std::string project_id,
            std::string database,
            std::string persistence_key,
            std::unique_ptr<auth::CredentialsProvider> credentials_provider,
            std::unique_ptr<util::AsyncQueue> worker_queue,
            void* extension);

  const model::DatabaseId& database_id() const {
    return database_id_;
  }

  const std::string& persistence_key() const {
    return persistence_key_;
  }

  FSTFirestoreClient* client() {
    HARD_ASSERT(client_, "Client is not yet configured.");
    return client_;
  }

  util::AsyncQueue* worker_queue();

  void* extension() {
    return extension_;
  }

  const Settings& settings() const;
  void set_settings(const Settings& settings);

  void set_user_executor(std::unique_ptr<util::Executor> user_executor);

  FIRCollectionReference* GetCollection(absl::string_view collection_path);
  DocumentReference GetDocument(absl::string_view document_path);
  WriteBatch GetBatch();
  FIRQuery* GetCollectionGroup(NSString* collection_id);

  void RunTransaction(core::TransactionUpdateCallback update_callback,
                      core::TransactionResultCallback result_callback);

  void Shutdown(util::StatusCallback callback);

  void EnableNetwork(util::StatusCallback callback);
  void DisableNetwork(util::StatusCallback callback);

 private:
  void EnsureClientConfigured();

  model::DatabaseId database_id_;
  std::unique_ptr<auth::CredentialsProvider> credentials_provider_;
  std::string persistence_key_;
  FSTFirestoreClient* client_ = nil;

  // Ownership of these will be transferred to `FSTFirestoreClient` as soon as
  // the client is created.
  std::unique_ptr<util::Executor> user_executor_;
  std::unique_ptr<util::AsyncQueue> worker_queue_;

  void* extension_ = nullptr;

  Settings settings_;

  mutable std::mutex mutex_;
};

}  // namespace api
}  // namespace firestore
}  // namespace firebase

NS_ASSUME_NONNULL_END

#endif  // FIRESTORE_CORE_SRC_FIREBASE_FIRESTORE_API_FIRESTORE_H_
