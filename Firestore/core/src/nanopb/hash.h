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

#ifndef FIRESTORE_CORE_SRC_NANOPB_HASH_H_
#define FIRESTORE_CORE_SRC_NANOPB_HASH_H_

#include <utility>
#include <vector>

#include "Firestore/Protos/nanopb/google/firestore/v1/document.nanopb.h"
#include "Firestore/core/src/nanopb/message.h"
#include "Firestore/core/src/nanopb/util.h"
#include "Firestore/core/src/util/hard_assert.h"

template <typename H>
H AbslHashValue(H h, const pb_bytes_array_t& obj) {
  return H::combine_contiguous(std::move(h), obj.bytes, obj.size);
}

namespace firebase {
namespace firestore {

template <typename H>
H AbslHashValue(H h, const google_firestore_v1_MapValue& obj) {
  const std::vector<google_firestore_v1_MapValue_FieldsEntry*> entries =
      nanopb::SortedEntries(obj);

  h = H::combine(std::move(h), entries.size());
  for (const google_firestore_v1_MapValue_FieldsEntry* entry : entries) {
    h = H::combine(std::move(h), *entry);
  }

  return h;
}

template <typename H>
H AbslHashValue(H h, const google_type_LatLng& obj) {
  return H::combine(std::move(h), obj);
}

template <typename H>
H AbslHashValue(H h, const google_firestore_v1_ArrayValue& obj) {
  return H::combine(std::move(h), obj);
}

template <typename H>
H AbslHashValue(H h, const google_protobuf_Timestamp& obj) {
  return H::combine(std::move(h), obj);
}

template <typename H>
H AbslHashValue(H h, const google_protobuf_NullValue& obj) {
  return H::combine(std::move(h), obj);
}

template <typename H>
H AbslHashValue(H h, const google_firestore_v1_Value& obj) {
  h = H::combine(std::move(h), obj.which_value_type);

  switch (obj.which_value_type) {
    case google_firestore_v1_Value_boolean_value_tag:
      return H::combine(std::move(h), obj.boolean_value);
    case google_firestore_v1_Value_integer_value_tag:
      return H::combine(std::move(h), obj.integer_value);
    case google_firestore_v1_Value_double_value_tag:
      return H::combine(std::move(h), obj.double_value);
    case google_firestore_v1_Value_reference_value_tag:
      return H::combine(std::move(h), *obj.reference_value);
    case google_firestore_v1_Value_map_value_tag:
      return H::combine(std::move(h), obj.map_value);
    case google_firestore_v1_Value_geo_point_value_tag:
      return H::combine(std::move(h), obj.geo_point_value);
    case google_firestore_v1_Value_array_value_tag:
      return H::combine(std::move(h), obj.array_value);
    case google_firestore_v1_Value_timestamp_value_tag:
      return H::combine(std::move(h), obj.timestamp_value);
    case google_firestore_v1_Value_null_value_tag:
      return H::combine(std::move(h), obj.null_value);
    case google_firestore_v1_Value_string_value_tag:
      return H::combine(std::move(h), *obj.string_value);
    case google_firestore_v1_Value_bytes_value_tag:
      return H::combine(std::move(h), *obj.bytes_value);
    default:
      HARD_FAIL("Invalid which_value_type in google_firestore_v1_Value: %s",
                obj.which_value_type);
  }

  UNREACHABLE();
}

}  // namespace firestore
}  // namespace firebase

#endif  // FIRESTORE_CORE_SRC_NANOPB_HASH_H_
