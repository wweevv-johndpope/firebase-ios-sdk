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

#include "Firestore/core/src/nanopb/util.h"

#include <algorithm>

#include "Firestore/Protos/nanopb/google/firestore/v1/document.nanopb.h"
#include "Firestore/core/src/nanopb/operators.h"

namespace firebase {
namespace firestore {
namespace nanopb {

std::vector<google_firestore_v1_MapValue_FieldsEntry*> SortedEntries(
    const google_firestore_v1_MapValue& obj) {
  std::vector<google_firestore_v1_MapValue_FieldsEntry*> entries;
  entries.reserve(obj.fields_count);
  for (decltype(obj.fields_count) i = 0; i < obj.fields_count; ++i) {
    entries.push_back(obj.fields + i);
  }

  struct {
    bool operator()(
        const google_firestore_v1_MapValue_FieldsEntry* entry1,
        const google_firestore_v1_MapValue_FieldsEntry* entry2) const {
      return (*entry1 < *entry2);
    }
  } customLess;

  std::sort(entries.begin(), entries.end(), customLess);
  return entries;
}

}  // namespace nanopb
}  // namespace firestore
}  // namespace firebase
