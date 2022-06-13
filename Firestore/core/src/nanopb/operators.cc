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

#include "Firestore/core/src/nanopb/operators.h"
#include "Firestore/core/src/nanopb/nanopb_util.h"
#include "Firestore/core/src/nanopb/util.h"

#include <algorithm>
#include <tuple>

namespace firebase {
namespace firestore {

namespace {

using nanopb::MakeStringView;
using nanopb::SortedEntries;

auto MakeTuple(const google_type_LatLng& obj)
    -> std::tuple<decltype(obj.latitude), decltype(obj.longitude)> {
  return std::make_tuple(obj.latitude, obj.longitude);
}

auto MakeTuple(const google_protobuf_Timestamp& obj)
    -> std::tuple<decltype(obj.seconds), decltype(obj.nanos)> {
  return std::make_tuple(obj.seconds, obj.nanos);
}

}  // namespace

bool operator<(const google_firestore_v1_ArrayValue& obj1,
               const google_firestore_v1_ArrayValue& obj2) {
  if (obj1.values_count < obj2.values_count) return true;
  if (obj2.values_count < obj1.values_count) return false;

  for (decltype(obj1.values_count) i = 0; i < obj1.values_count; ++i) {
    const google_firestore_v1_Value* value1 = obj1.values + i;
    const google_firestore_v1_Value* value2 = obj2.values + i;
    if (*value1 < *value2) return true;
    if (*value2 < *value1) return false;
  }

  return false;
}

bool operator<(const google_firestore_v1_MapValue& obj1,
               const google_firestore_v1_MapValue& obj2) {
  if (obj1.fields_count < obj2.fields_count) return true;
  if (obj2.fields_count < obj1.fields_count) return false;

  const std::vector<google_firestore_v1_MapValue_FieldsEntry*> entries1 =
      SortedEntries(obj1);
  const std::vector<google_firestore_v1_MapValue_FieldsEntry*> entries2 =
      SortedEntries(obj2);

  for (decltype(obj1.fields_count) i = 0; i < obj1.fields_count; ++i) {
    const google_firestore_v1_MapValue_FieldsEntry* entry1 = entries1[i];
    const google_firestore_v1_MapValue_FieldsEntry* entry2 = entries2[i];
    if (*entry1 < *entry2) return true;
    if (*entry2 < *entry1) return false;
  }

  return false;
}

bool operator<(const google_firestore_v1_MapValue_FieldsEntry& obj1,
               const google_firestore_v1_MapValue_FieldsEntry& obj2) {
  const absl::string_view key1 = MakeStringView(obj1.key);
  const absl::string_view key2 = MakeStringView(obj2.key);
  if (key1 != key2) {
    return key1 < key2;
  }

  return obj1.value < obj2.value;
}

bool operator<(const google_firestore_v1_Value& obj1,
               const google_firestore_v1_Value& obj2) {
  if (obj1.which_value_type != obj2.which_value_type) {
    return obj1.which_value_type < obj2.which_value_type;
  }

  switch (obj1.which_value_type) {
    case google_firestore_v1_Value_boolean_value_tag:
      return obj1.boolean_value < obj2.boolean_value;
    case google_firestore_v1_Value_integer_value_tag:
      return obj1.integer_value < obj2.integer_value;
    case google_firestore_v1_Value_double_value_tag:
      return obj1.double_value < obj2.double_value;
    case google_firestore_v1_Value_reference_value_tag:
      return MakeStringView(obj1.reference_value) <
             MakeStringView(obj2.reference_value);
    case google_firestore_v1_Value_map_value_tag:
      return obj1.map_value < obj2.map_value;
    case google_firestore_v1_Value_geo_point_value_tag:
      return obj1.geo_point_value < obj2.geo_point_value;
    case google_firestore_v1_Value_array_value_tag:
      return obj1.array_value < obj2.array_value;
    case google_firestore_v1_Value_timestamp_value_tag:
      return obj1.timestamp_value < obj2.timestamp_value;
    case google_firestore_v1_Value_null_value_tag:
      return obj1.null_value < obj2.null_value;
    case google_firestore_v1_Value_string_value_tag:
      return MakeStringView(obj1.string_value) <
             MakeStringView(obj2.string_value);
    case google_firestore_v1_Value_bytes_value_tag:
      return MakeStringView(obj1.bytes_value) <
             MakeStringView(obj2.bytes_value);
    default:
      HARD_FAIL("Invalid which_value_type in google_firestore_v1_Value: %s",
                obj1.which_value_type);
  }

  UNREACHABLE();
}

bool operator<(const google_protobuf_Timestamp& obj1,
               const google_protobuf_Timestamp& obj2) {
  return MakeTuple(obj1) < MakeTuple(obj2);
}

bool operator<(const google_type_LatLng& obj1, const google_type_LatLng& obj2) {
  return MakeTuple(obj1) < MakeTuple(obj2);
}

}  // namespace firestore
}  // namespace firebase