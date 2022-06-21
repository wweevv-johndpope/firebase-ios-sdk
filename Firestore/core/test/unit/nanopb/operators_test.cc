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

#include "Firestore/Protos/nanopb/google/firestore/v1/document.nanopb.h"

#include "Firestore/core/include/firebase/firestore/geo_point.h"
#include "Firestore/core/src/nanopb/byte_string.h"
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
using testutil::MapValue_FieldsEntry;
using testutil::Ref;
using testutil::Value;

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_DifferentLengths) {
  Message<google_firestore_v1_ArrayValue> short_array = Array(9);
  Message<google_firestore_v1_ArrayValue> long_array = Array(1, 2, 3, 4, 5);
  EXPECT_TRUE(*short_array < *long_array);
  EXPECT_FALSE(*long_array < *short_array);
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_EqualArrays) {
  EXPECT_FALSE(*Array() < *Array());
  EXPECT_FALSE(*Array(true) < *Array(true));
  EXPECT_FALSE(*Array(false) < *Array(false));
  EXPECT_FALSE(*Array(1) < *Array(1));
  EXPECT_FALSE(*Array(5.0) < *Array(5.0));
  EXPECT_FALSE(*Array(Ref("a", "b")) < *Array(Ref("a", "b")));
  EXPECT_FALSE(*Array(Map("key", 42)) < *Array(Map("key", 42)));
  EXPECT_FALSE(*Array(GeoPoint(1, 2)) < *Array(GeoPoint(1, 2)));
  EXPECT_FALSE(*Array(Array(42)) < *Array(Array(42)));
  EXPECT_FALSE(*Array(Timestamp(1, 2)) < *Array(Timestamp(1, 2)));
  EXPECT_FALSE(*Array(nullptr) < *Array(nullptr));
  EXPECT_FALSE(*Array("string") < *Array("string"));
  EXPECT_FALSE(*Array(ByteString()) < *Array(ByteString()));
  EXPECT_FALSE(*Array(12, 5.0, false, "abc") < *Array(12, 5.0, false, "abc"));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_DifferentTypes) {
  EXPECT_TRUE(*Array(true) < *Array(42));
  EXPECT_FALSE(*Array(42) < *Array(true));
  EXPECT_TRUE(*Array(42) < *Array(5.0));
  EXPECT_FALSE(*Array(5.0) < *Array(42));
  EXPECT_TRUE(*Array(5.0) < *Array(Ref("a", "b")));
  EXPECT_FALSE(*Array(Ref("a", "b")) < *Array(5.0));
  EXPECT_TRUE(*Array(Ref("a", "b")) < *Array(Map()));
  EXPECT_FALSE(*Array(Map()) < *Array(Ref("a", "b")));
  EXPECT_TRUE(*Array(Map()) < *Array(GeoPoint(1, 1)));
  EXPECT_FALSE(*Array(GeoPoint(1, 1)) < *Array(Map()));
  EXPECT_TRUE(*Array(GeoPoint(1, 1)) < *Array(Array()));
  EXPECT_FALSE(*Array(Array()) < *Array(GeoPoint(1, 1)));
  EXPECT_TRUE(*Array(Array()) < *Array(Timestamp(1, 1)));
  EXPECT_FALSE(*Array(Timestamp(1, 1)) < *Array(Array()));
  EXPECT_TRUE(*Array(Timestamp(1, 1)) < *Array(Value(nullptr)));
  EXPECT_FALSE(*Array(Value(nullptr)) < *Array(Timestamp(1, 1)));
  EXPECT_TRUE(*Array(Value(nullptr)) < *Array("string"));
  EXPECT_FALSE(*Array("string") < *Array(Value(nullptr)));
  EXPECT_TRUE(*Array("string") < *Array(ByteString("byte")));
  EXPECT_FALSE(*Array(ByteString("byte")) < *Array("string"));
}

TEST(NanopbOperatorsTest,
     ArrayValue_OperatorLessThan_ResultIsFirstUnequalValue) {
  EXPECT_TRUE(*Array(1, 2, 3) < *Array(1, 2, 99));
  EXPECT_FALSE(*Array(1, 2, 99) < *Array(1, 2, 3));
  EXPECT_TRUE(*Array(1, 2, 3) < *Array(1, 99, -100));
  EXPECT_FALSE(*Array(1, 99, -100) < *Array(1, 2, 3));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_BooleanValues) {
  EXPECT_TRUE(*Array(false) < *Array(true));
  EXPECT_FALSE(*Array(true) < *Array(false));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_IntegerValues) {
  EXPECT_TRUE(*Array(1) < *Array(2));
  EXPECT_FALSE(*Array(2) < *Array(1));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_DoubleValues) {
  EXPECT_TRUE(*Array(1.0) < *Array(2.0));
  EXPECT_FALSE(*Array(2.0) < *Array(1.0));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_ReferenceValues) {
  EXPECT_TRUE(*Array(Ref("a", "b")) < *Array(Ref("x", "y")));
  EXPECT_FALSE(*Array(Ref("x", "y")) < *Array(Ref("a", "b")));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_MapValues) {
  EXPECT_TRUE(*Array(Map("k1", "v1")) < *Array(Map("k2", "v2")));
  EXPECT_FALSE(*Array(Map("k2", "v2")) < *Array(Map("k1", "v1")));
  EXPECT_TRUE(*Array(Map()) < *Array(Map("k2", "v2")));
  EXPECT_FALSE(*Array(Map("k2", "v2")) < *Array(Map()));
  EXPECT_TRUE(*Array(Map("k2", 0, "k1", 0)) < *Array(Map("k1", 0, "k3", 0)));
  EXPECT_FALSE(*Array(Map("k1", 0, "k3", 0)) < *Array(Map("k2", 0, "k1", 0)));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_GeoPointValues) {
  EXPECT_TRUE(*Array(GeoPoint(1, 2)) < *Array(GeoPoint(2, 3)));
  EXPECT_FALSE(*Array(GeoPoint(2, 3)) < *Array(GeoPoint(1, 2)));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_ArrayValues) {
  EXPECT_TRUE(*Array(Array()) < *Array(Array(1)));
  EXPECT_FALSE(*Array(Array(1)) < *Array(Array()));
  EXPECT_TRUE(*Array(Array(0)) < *Array(Array(1)));
  EXPECT_FALSE(*Array(Array(1)) < *Array(Array(0)));
  EXPECT_TRUE(*Array(Array(0, 1, 2)) < *Array(Array(0, 1, 99)));
  EXPECT_FALSE(*Array(Array(0, 1, 99)) < *Array(Array(0, 1, 2)));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_TimestampValues) {
  EXPECT_TRUE(*Array(Timestamp(1, 2)) < *Array(Timestamp(2, 3)));
  EXPECT_FALSE(*Array(Timestamp(2, 3)) < *Array(Timestamp(1, 2)));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_NullValues) {
  EXPECT_FALSE(*Array(Value(nullptr)) < *Array(Value(nullptr)));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_StringValues) {
  EXPECT_TRUE(*Array("elem1") < *Array("elem2"));
  EXPECT_FALSE(*Array("elem2") < *Array("elem1"));
}

TEST(NanopbOperatorsTest, ArrayValue_OperatorLessThan_ByteStringValues) {
  EXPECT_TRUE(*Array(ByteString("elem1")) < *Array(ByteString("elem2")));
  EXPECT_FALSE(*Array(ByteString("elem2")) < *Array(ByteString("elem1")));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_DifferentLengths) {
  Message<google_firestore_v1_MapValue> short_map = MapValue("Z", 42);
  Message<google_firestore_v1_MapValue > long_map = MapValue("A", 42, "B", 42);
  EXPECT_TRUE(*short_map < *long_map);
  EXPECT_FALSE(*long_map < *short_map);
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_EqualMaps) {
  EXPECT_FALSE(*MapValue() < *MapValue());
  EXPECT_FALSE(*MapValue("key", true) < *MapValue("key", true));
  EXPECT_FALSE(*MapValue("key", false) < *MapValue("key", false));
  EXPECT_FALSE(*MapValue("key", 1) < *MapValue("key", 1));
  EXPECT_FALSE(*MapValue("key", 5.0) < *MapValue("key", 5.0));
  EXPECT_FALSE(*MapValue("key", Ref("a", "b")) < *MapValue("key", Ref("a", "b")));
  EXPECT_FALSE(*MapValue("key", Map("key", 42)) < *MapValue("key", Map("key", 42)));
  EXPECT_FALSE(*MapValue("key", GeoPoint(1, 2)) < *MapValue("key", GeoPoint(1, 2)));
  EXPECT_FALSE(*MapValue("key", MapValue("abc", 42)) < *MapValue("key", MapValue("abc", 42)));
  EXPECT_FALSE(*MapValue("key", Timestamp(1, 2)) < *MapValue("key", Timestamp(1, 2)));
  EXPECT_FALSE(*MapValue("key", nullptr) < *MapValue("key", nullptr));
  EXPECT_FALSE(*MapValue("key", "string") < *MapValue("key", "string"));
  EXPECT_FALSE(*MapValue("key", ByteString()) < *MapValue("key", ByteString()));
  EXPECT_FALSE(*MapValue("key1", 12, "key2", 5.0, "key3", false, "key4", "abc") < *MapValue("key1", 12, "key2", 5.0, "key3", false, "key4", "abc"));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_DifferentKeys) {
  EXPECT_TRUE(*MapValue("A", 42) < *MapValue("Z", 42));
  EXPECT_FALSE(*MapValue("Z", 42) < *MapValue("A", 42));
  EXPECT_TRUE(*MapValue("A", 42, "B", 42) < *MapValue("A", 42, "Z", 42));
  EXPECT_FALSE(*MapValue("A", 42, "Z", 42) < *MapValue("A", 42, "B", 42));
  EXPECT_TRUE(*MapValue("A", 42, "B", 42, "C", 42) < *MapValue("A", 42, "B", 42, "Z", 42));
  EXPECT_FALSE(*MapValue("A", 42, "B", 42, "Z", 42) < *MapValue("A", 42, "B", 42, "C", 42));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_DifferentKeys_OrderIsIgnored) {
  EXPECT_TRUE(*MapValue("B", 42, "A", 42) < *MapValue("A", 42, "Z", 42));
  EXPECT_FALSE(*MapValue("A", 42, "Z", 42) < *MapValue("B", 42, "A", 42));
  EXPECT_TRUE(*MapValue("A", 42, "B", 42) < *MapValue("Z", 42, "A", 42));
  EXPECT_FALSE(*MapValue("Z", 42, "A", 42) < *MapValue("A", 42, "B", 42));

  EXPECT_TRUE(*MapValue("A", 42, "C", 42, "B", 42) < *MapValue("A", 42, "B", 42, "Z", 42));
  EXPECT_FALSE(*MapValue("A", 42, "B", 42, "Z", 42) < *MapValue("A", 42, "C", 42, "B", 42));
  EXPECT_TRUE(*MapValue("C", 42, "B", 42, "A", 42) < *MapValue("A", 42, "B", 42, "Z", 42));
  EXPECT_FALSE(*MapValue("A", 42, "B", 42, "Z", 42) < *MapValue("C", 42, "B", 42, "A", 42));
  EXPECT_TRUE(*MapValue("C", 42, "A", 42, "B", 42) < *MapValue("A", 42, "B", 42, "Z", 42));
  EXPECT_FALSE(*MapValue("A", 42, "B", 42, "Z", 42) < *MapValue("A", 42, "B", 42, "C", 42));

  EXPECT_TRUE(*MapValue("A", 42, "B", 42, "C", 42) < *MapValue("A", 42, "Z", 42, "B", 42));
  EXPECT_FALSE(*MapValue("A", 42, "Z", 42, "B", 42) < *MapValue("A", 42, "B", 42, "C", 42));
  EXPECT_TRUE(*MapValue("A", 42, "B", 42, "C", 42) < *MapValue("Z", 42, "B", 42, "A", 42));
  EXPECT_FALSE(*MapValue("Z", 42, "B", 42, "A", 42) < *MapValue("A", 42, "B", 42, "C", 42));
  EXPECT_TRUE(*MapValue("A", 42, "B", 42, "C", 42) < *MapValue("Z", 42, "A", 42, "B", 42));
  EXPECT_FALSE(*MapValue("Z", 42, "A", 42, "B", 42) < *MapValue("A", 42, "B", 42, "C", 42));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_DifferentTypes) {
  EXPECT_TRUE(*MapValue("key", true) < *MapValue("key", 42));
  EXPECT_FALSE(*MapValue("key", 42) < *MapValue("key", true));
  EXPECT_TRUE(*MapValue("key", 42) < *MapValue("key", 5.0));
  EXPECT_FALSE(*MapValue("key", 5.0) < *MapValue("key", 42));
  EXPECT_TRUE(*MapValue("key", 5.0) < *MapValue("key", Ref("a", "b")));
  EXPECT_FALSE(*MapValue("key", Ref("a", "b")) < *MapValue("key", 5.0));
  EXPECT_TRUE(*MapValue("key", Ref("a", "b")) < *MapValue("key", Map()));
  EXPECT_FALSE(*MapValue("key", Map()) < *MapValue("key", Ref("a", "b")));
  EXPECT_TRUE(*MapValue("key", Map()) < *MapValue("key", GeoPoint(1, 1)));
  EXPECT_FALSE(*MapValue("key", GeoPoint(1, 1)) < *MapValue("key", Map()));
  EXPECT_TRUE(*MapValue("key", GeoPoint(1, 1)) < *MapValue("key", Array()));
  EXPECT_FALSE(*MapValue("key", Array()) < *MapValue("key", GeoPoint(1, 1)));
  EXPECT_TRUE(*MapValue("key", Array()) < *MapValue("key", Timestamp(1, 1)));
  EXPECT_FALSE(*MapValue("key", Timestamp(1, 1)) < *MapValue("key", Array()));
  EXPECT_TRUE(*MapValue("key", Timestamp(1, 1)) < *MapValue("key", Value(nullptr)));
  EXPECT_FALSE(*MapValue("key", Value(nullptr)) < *MapValue("key", Timestamp(1, 1)));
  EXPECT_TRUE(*MapValue("key", Value(nullptr)) < *MapValue("key", "string"));
  EXPECT_FALSE(*MapValue("key", "string") < *MapValue("key", Value(nullptr)));
  EXPECT_TRUE(*MapValue("key", "string") < *MapValue("key", ByteString("byte")));
  EXPECT_FALSE(*MapValue("key", ByteString("byte")) < *MapValue("key", "string"));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_BooleanValues) {
  EXPECT_TRUE(*MapValue("key", false) < *MapValue("key", true));
  EXPECT_FALSE(*MapValue("key", true) < *MapValue("key", false));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_IntegerValues) {
  EXPECT_TRUE(*MapValue("key", 1) < *MapValue("key", 2));
  EXPECT_FALSE(*MapValue("key", 2) < *MapValue("key", 1));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_DoubleValues) {
  EXPECT_TRUE(*MapValue("key", 1.0) < *MapValue("key", 2.0));
  EXPECT_FALSE(*MapValue("key", 2.0) < *MapValue("key", 1.0));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_ReferenceValues) {
  EXPECT_TRUE(*MapValue("key", Ref("a", "b")) < *MapValue("key", Ref("x", "y")));
  EXPECT_FALSE(*MapValue("key", Ref("x", "y")) < *MapValue("key", Ref("a", "b")));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_MapValues) {
  EXPECT_TRUE(*MapValue("key", Map("k1", "v1")) < *MapValue("key", Map("k2", "v2")));
  EXPECT_FALSE(*MapValue("key", Map("k2", "v2")) < *MapValue("key", Map("k1", "v1")));
  EXPECT_TRUE(*MapValue("key", Map()) < *MapValue("key", Map("k2", "v2")));
  EXPECT_FALSE(*MapValue("key", Map("k2", "v2")) < *MapValue("key", Map()));
  EXPECT_TRUE(*MapValue("key", Map("k2", 0, "k1", 0)) < *MapValue("key", Map("k1", 0, "k3", 0)));
  EXPECT_FALSE(*MapValue("key", Map("k1", 0, "k3", 0)) < *MapValue("key", Map("k2", 0, "k1", 0)));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_GeoPointValues) {
  EXPECT_TRUE(*MapValue("key", GeoPoint(1, 2)) < *MapValue("key", GeoPoint(2, 3)));
  EXPECT_FALSE(*MapValue("key", GeoPoint(2, 3)) < *MapValue("key", GeoPoint(1, 2)));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_ArrayValues) {
  EXPECT_TRUE(*MapValue("key", Array()) < *MapValue("key", Array(1)));
  EXPECT_FALSE(*MapValue("key", Array(1)) < *MapValue("key", Array()));
  EXPECT_TRUE(*MapValue("key", Array(0)) < *MapValue("key", Array(1)));
  EXPECT_FALSE(*MapValue("key", Array(1)) < *MapValue("key", Array(0)));
  EXPECT_TRUE(*MapValue("key", Array(0, 1, 2)) < *MapValue("key", Array(0, 1, 99)));
  EXPECT_FALSE(*MapValue("key", Array(0, 1, 99)) < *MapValue("key", Array(0, 1, 2)));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_TimestampValues) {
  EXPECT_TRUE(*MapValue("key", Timestamp(1, 2)) < *MapValue("key", Timestamp(2, 3)));
  EXPECT_FALSE(*MapValue("key", Timestamp(2, 3)) < *MapValue("key", Timestamp(1, 2)));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_NullValues) {
  EXPECT_FALSE(*MapValue("key", Value(nullptr)) < *MapValue("key", Value(nullptr)));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_StringValues) {
  EXPECT_TRUE(*MapValue("key", "elem1") < *MapValue("key", "elem2"));
  EXPECT_FALSE(*MapValue("key", "elem2") < *MapValue("key", "elem1"));
}

TEST(NanopbOperatorsTest, MapValue_OperatorLessThan_ByteStringValues) {
  EXPECT_TRUE(*MapValue("key", ByteString("elem1")) < *MapValue("key", ByteString("elem2")));
  EXPECT_FALSE(*MapValue("key", ByteString("elem2")) < *MapValue("key", ByteString("elem1")));
}

TEST(NanopbOperatorsTest, MapValue_FieldsEntry_OperatorLessThan_DifferentKeys) {
  EXPECT_TRUE(*MapValue_FieldsEntry("key1", 42) < *MapValue_FieldsEntry("key2", 42));
  EXPECT_FALSE(*MapValue_FieldsEntry("key2", 42) < *MapValue_FieldsEntry("key1", 42));
}

TEST(NanopbOperatorsTest, MapValue_FieldsEntry_OperatorLessThan_EqualInstances) {
  EXPECT_FALSE(*MapValue_FieldsEntry("key", true) < *MapValue_FieldsEntry("key", true));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", false) < *MapValue_FieldsEntry("key", false));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", 1) < *MapValue_FieldsEntry("key", 1));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", 5.0) < *MapValue_FieldsEntry("key", 5.0));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", Ref("a", "b")) < *MapValue_FieldsEntry("key", Ref("a", "b")));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", Map("key", 42)) < *MapValue_FieldsEntry("key", Map("key", 42)));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", GeoPoint(1, 2)) < *MapValue_FieldsEntry("key", GeoPoint(1, 2)));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", Array(42)) < *MapValue_FieldsEntry("key", Array(42)));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", Timestamp(1, 2)) < *MapValue_FieldsEntry("key", Timestamp(1, 2)));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", nullptr) < *MapValue_FieldsEntry("key", nullptr));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", "string") < *MapValue_FieldsEntry("key", "string"));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", ByteString()) < *MapValue_FieldsEntry("key", ByteString()));
}

TEST(NanopbOperatorsTest, MapValue_FieldsEntry_OperatorLessThan_DifferentValueTypes) {
  EXPECT_TRUE(*MapValue_FieldsEntry("key", true) < *MapValue_FieldsEntry("key", 42));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", 42) < *MapValue_FieldsEntry("key", true));
  EXPECT_TRUE(*MapValue_FieldsEntry("key", 42) < *MapValue_FieldsEntry("key", 5.0));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", 5.0) < *MapValue_FieldsEntry("key", 42));
  EXPECT_TRUE(*MapValue_FieldsEntry("key", 5.0) < *MapValue_FieldsEntry("key", Ref("a", "b")));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", Ref("a", "b")) < *MapValue_FieldsEntry("key", 5.0));
  EXPECT_TRUE(*MapValue_FieldsEntry("key", Ref("a", "b")) < *MapValue_FieldsEntry("key", Map()));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", Map()) < *MapValue_FieldsEntry("key", Ref("a", "b")));
  EXPECT_TRUE(*MapValue_FieldsEntry("key", Map()) < *MapValue_FieldsEntry("key", GeoPoint(1, 1)));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", GeoPoint(1, 1)) < *MapValue_FieldsEntry("key", Map()));
  EXPECT_TRUE(*MapValue_FieldsEntry("key", GeoPoint(1, 1)) < *MapValue_FieldsEntry("key", Array()));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", Array()) < *MapValue_FieldsEntry("key", GeoPoint(1, 1)));
  EXPECT_TRUE(*MapValue_FieldsEntry("key", Array()) < *MapValue_FieldsEntry("key", Timestamp(1, 1)));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", Timestamp(1, 1)) < *MapValue_FieldsEntry("key", Array()));
  EXPECT_TRUE(*MapValue_FieldsEntry("key", Timestamp(1, 1)) < *MapValue_FieldsEntry("key", Value(nullptr)));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", Value(nullptr)) < *MapValue_FieldsEntry("key", Timestamp(1, 1)));
  EXPECT_TRUE(*MapValue_FieldsEntry("key", Value(nullptr)) < *MapValue_FieldsEntry("key", "string"));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", "string") < *MapValue_FieldsEntry("key", Value(nullptr)));
  EXPECT_TRUE(*MapValue_FieldsEntry("key", "string") < *MapValue_FieldsEntry("key", ByteString("byte")));
  EXPECT_FALSE(*MapValue_FieldsEntry("key", ByteString("byte")) < *MapValue_FieldsEntry("key", "string"));
}

}  //  namespace
}  //  namespace nanopb
}  //  namespace firestore
}  //  namespace firebase
