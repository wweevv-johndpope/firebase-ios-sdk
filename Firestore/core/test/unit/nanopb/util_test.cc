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

#include <functional>

#include "Firestore/core/include/firebase/firestore/geo_point.h"
#include "Firestore/core/src/nanopb/message.h"
#include "Firestore/core/test/unit/testutil/testutil.h"

#include "gtest/gtest.h"

namespace firebase {
namespace firestore {
namespace nanopb {
namespace {

using nanopb::ByteString;
using testutil::Array;
using testutil::Map;
using testutil::MapValue;
using testutil::Ref;

/**
 * Creates a std::vector containing pointers to the field entries of a Map.
 * @param map_value the map whose field entry pointers to copy into the returned
 * vector.
 * @param indices the indexes into the given `map_value.fields` array of the
 * elements whose pointers to copy into the returned vector, in the order
 * desired.
 * @return pointers to the field entries from the given map in the order
 * specified by the given `indices` vector.
 */
std::vector<google_firestore_v1_MapValue_FieldsEntry*> VectorWithEntries(
    const google_firestore_v1_MapValue& map_value,
    const std::vector<int>& indices) {
  std::vector<google_firestore_v1_MapValue_FieldsEntry*> result;
  result.reserve(indices.size());
  for (const int index : indices) {
    result.push_back(&map_value.fields[index]);
  }
  return result;
}

void AssertFieldTypes(const google_firestore_v1_MapValue& value,
                      int expected_type) {
  for (decltype(value.fields_count) i = 0; i < value.fields_count; ++i) {
    ASSERT_EQ(value.fields[i].value.which_value_type, expected_type);
  }
}

TEST(NanopbSortedEntriesTest, ZeroEntries) {
  Message<google_firestore_v1_MapValue> map_value = MapValue();

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  ASSERT_EQ(sorted_entries.size(), 0);
}

TEST(NanopbSortedEntriesTest, OneEntry) {
  Message<google_firestore_v1_MapValue> map_value = MapValue("TestKey", 42);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {0}));
}

TEST(NanopbSortedEntriesTest, TwoEntries_DifferenteKeys_AlreadySorted) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("key1", 42, "key2", 41);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {0, 1}));
}

TEST(NanopbSortedEntriesTest, TwoEntries_DifferenteKeys_NotSorted) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("key2", 42, "key1", 43);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {1, 0}));
}

TEST(NanopbSortedEntriesTest, SameKeys_DifferentValueType) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("Key", Map("key", "value"), "Key", GeoPoint(1.0, 1.0), "Key",
               ByteString("byte string"), "Key", 5, "Key", 5.0, "Key",
               Timestamp(1, 2), "Key", "string value", "Key", false, "Key",
               Ref("project", "path"), "Key", Array(1, 2, 3), "Key", nullptr);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries,
            VectorWithEntries(*map_value, {7, 3, 4, 8, 0, 1, 9, 5, 10, 6, 2}));
}

TEST(NanopbSortedEntriesTest, SortsBooleanValues) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("Key", true, "Key", false);
  AssertFieldTypes(*map_value, google_firestore_v1_Value_boolean_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {1, 0}));
}

TEST(NanopbSortedEntriesTest, SortsIntegerValues) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("Key", 5, "Key", -100, "Key", 500);
  AssertFieldTypes(*map_value, google_firestore_v1_Value_integer_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {1, 0, 2}));
}

TEST(NanopbSortedEntriesTest, SortsDoubleValues) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("Key", 5.0, "Key", -100.0, "Key", 500.0);
  AssertFieldTypes(*map_value, google_firestore_v1_Value_double_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {1, 0, 2}));
}

TEST(NanopbSortedEntriesTest, SortsReferenceValues) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("Key", Ref("2", "1"), "Key", Ref("3", "0"), "Key", Ref("2", "0"),
               "Key", Ref("1", "1"));
  AssertFieldTypes(*map_value, google_firestore_v1_Value_reference_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {3, 2, 0, 1}));
}

TEST(NanopbSortedEntriesTest, SortsMapValues_DifferentSizes) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("Key", Map("1", "2", "3", "4", "5", "6"), "Key", Map("3", "4"),
               "Key", Map(), "Key", Map("2", "3", "4", "5"));
  AssertFieldTypes(*map_value, google_firestore_v1_Value_map_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  // Maps with smaller numbers of entries are always sorted before those with
  // larger numbers of entries, ignoring their contents.
  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {2, 1, 3, 0}));
}

TEST(NanopbSortedEntriesTest, SortsMapValues_SameSizes_DifferentValueTypes) {
  Message<google_firestore_v1_MapValue> map_value = MapValue(
      "Key", Map("key", "value"), "Key", Map("key", 42), "Key",
      Map("key", true), "Key", Map("key", nullptr), "Key", Map("key", 6.0));

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {2, 1, 4, 3, 0}));
}

TEST(NanopbSortedEntriesTest, SortsMapValues_SameSizes_DifferentKeys) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("Key", Map("keyX", 0, "keyB", 0, "keyC", 0), "Key",
               Map("keyA", 0, "keyB", 0, "keyD", 0), "Key",
               Map("keyA", 0, "keyB", 0, "keyC", 0), "Key",
               Map("keyE", 0, "keyB", 0, "keyA", 0));
  AssertFieldTypes(*map_value, google_firestore_v1_Value_map_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  // Maps with the same size are compared lexicographically after sorting the
  // keys; therefore, the order of the fields is not relevant.
  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {2, 1, 3, 0}));
}

TEST(NanopbSortedEntriesTest, SortsGeoPointValues) {
  Message<google_firestore_v1_MapValue> map_value = MapValue(
      "Key", GeoPoint(2.0, 3.0), "Key", GeoPoint(4.0, -1.0), "Key",
      GeoPoint(1.0, 2.0), "Key", GeoPoint(2.0, 4.0), "Key", GeoPoint(3.0, 1.0));
  AssertFieldTypes(*map_value, google_firestore_v1_Value_geo_point_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  // Arrays with smaller numbers of elements are always sorted before those with
  // larger numbers of elements, ignoring their contents.
  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {2, 0, 3, 4, 1}));
}

TEST(NanopbSortedEntriesTest, SortsArrayValues_DifferentLengths) {
  Message<google_firestore_v1_MapValue> map_value = MapValue(
      "Key", Array(8, 7), "Key", Array(3, 2, 1, 0, -1), "Key", Array(9), "Key",
      Array(4, 3, 2, 1), "Key", Array(7, 6, 5), "Key", Array());
  AssertFieldTypes(*map_value, google_firestore_v1_Value_array_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {5, 2, 0, 4, 3, 1}));
}

TEST(NanopbSortedEntriesTest,
     SortsArrayValues_SameLengths_DifferentValueTypes) {
  Message<google_firestore_v1_MapValue> map_value = MapValue(
      "Key", Array(1, 2, "value"), "Key", Array(1, 2, 42), "Key",
      Array(1, 2, true), "Key", Array(1, 2, nullptr), "Key", Array(1, 2, 6.0));

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {2, 1, 4, 3, 0}));
}

TEST(NanopbSortedEntriesTest, SortsArrayValues_SameLengths_SameValueTypes) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("Key", Array(1, 2, 9, 9, 9), "Key", Array(1, 2, 3, 4, 8), "Key",
               Array(5, 4, 3, 2, 1), "Key", Array(1, 2, 3, 8, 8), "Key",
               Array(1, 2, 8, 8, 8), "Key", Array(1, 2, 3, 4, 5));
  AssertFieldTypes(*map_value, google_firestore_v1_Value_array_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  // Maps with the same size are compared lexicographically without any sorting.
  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {5, 1, 3, 4, 0, 2}));
}

TEST(NanopbSortedEntriesTest, SortsTimestampValues) {
  Message<google_firestore_v1_MapValue> map_value = MapValue(
      "Key", Timestamp(2, 1), "Key", Timestamp(1, 2), "Key", Timestamp(3, 0),
      "Key", Timestamp(2, 0), "Key", Timestamp(1, 4), "Key", Timestamp(1, 3));
  AssertFieldTypes(*map_value, google_firestore_v1_Value_timestamp_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {1, 5, 4, 3, 0, 2}));
}

TEST(NanopbSortedEntriesTest, SortsStringValues) {
  Message<google_firestore_v1_MapValue> map_value =
      MapValue("Key", "string5", "Key", "string2", "Key", "string4", "Key",
               "string1", "Key", "string3");
  AssertFieldTypes(*map_value, google_firestore_v1_Value_string_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {3, 1, 4, 2, 0}));
}

TEST(NanopbSortedEntriesTest, SortsByteValues) {
  Message<google_firestore_v1_MapValue> map_value = MapValue(
      "Key", ByteString("ByteString5"), "Key", ByteString("ByteString2"), "Key",
      ByteString("ByteString4"), "Key", ByteString("ByteString1"), "Key",
      ByteString("ByteString3"));
  AssertFieldTypes(*map_value, google_firestore_v1_Value_bytes_value_tag);

  std::vector<google_firestore_v1_MapValue_FieldsEntry*> sorted_entries =
      SortedEntries(*map_value);

  EXPECT_EQ(sorted_entries, VectorWithEntries(*map_value, {3, 1, 4, 2, 0}));
}

}  //  namespace
}  //  namespace nanopb
}  //  namespace firestore
}  //  namespace firebase
