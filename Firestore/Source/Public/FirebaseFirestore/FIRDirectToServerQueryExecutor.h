/*
 * Copyright 2022 Google LLC
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

#import <Foundation/Foundation.h>

@class FIRAggregateQuery;
@class FIRAggregateQuerySnapshot;
@class FIRDocumentReference;
@class FIRDocumentSnapshot;
@class FIRGroupByQuery;
@class FIRGroupByQuerySnapshot;
@class FIRPartitionQuery;
@class FIRPartitionQuerySnapshot;
@class FIRQuery;
@class FIRQuerySnapshot;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, FIROnlineState) {
  FIRFirestoreOnline,
  FIRFirestoreOffline
} NS_SWIFT_NAME(OnlineState);

NS_SWIFT_NAME(DirectToServerQueryExecutor)
@interface FIRDirectToServerQueryExecutor<T> : NSObject

/** :nodoc: */
- (id)init
    __attribute__((unavailable("FIRDirectToServerQueryExecutor cannot be created directly.")));

+ (FIRDirectToServerQueryExecutor<FIRDocumentSnapshot *> *)DirectToServerQueryExecutorForDocument:
    (FIRDocumentReference *)doc;

+ (FIRDirectToServerQueryExecutor<FIRQuerySnapshot *> *)DirectToServerQueryExecutorForQuery:
    (FIRQuery *)query;

+ (FIRDirectToServerQueryExecutor<FIRAggregateQuerySnapshot *> *)
    DirectToServerQueryExecutorForAggregateQuery:(FIRAggregateQuery *)query;

+ (FIRDirectToServerQueryExecutor<FIRGroupByQuerySnapshot *> *)
    DirectToServerQueryExecutorForGroupByQuery:(FIRGroupByQuery *)query;

+ (FIRDirectToServerQueryExecutor<FIRPartitionQuerySnapshot *> *)
    DirectToServerQueryExecutorForPartitionQuery:(FIRPartitionQuery *)query;

- (void)getWithCompletion:(void (^)(T *_Nullable snapshot, NSError *_Nullable error))completion
    NS_SWIFT_NAME(get(completion:));

- (id<FIRListenerRegistration>)addSnapshotListener:
    (void (^)(T *_Nullable snapshot, NSError *_Nullable error, FIROnlineState onlineState))listener
    NS_SWIFT_NAME(addSnapshotListener(_:));

// ALTERNATIVE

- (id<FIRListenerRegistration>)
          addSnapshotListener:(void (^)(T *_Nullable snapshot, NSError *_Nullable error))listener
    onlineStateChangeListener:(void (^)(FIROnlineState onlineState))onlineStateChangeListener
    NS_SWIFT_NAME(addSnapshotListener(listener:onlineStateChangeListener:));

@end

NS_ASSUME_NONNULL_END
