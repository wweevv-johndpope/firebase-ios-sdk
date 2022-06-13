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

#ifndef FIRESTORE_CORE_SRC_NANOPB_OPERATORS_H_
#define FIRESTORE_CORE_SRC_NANOPB_OPERATORS_H_

#include "Firestore/Protos/nanopb/google/firestore/v1/document.nanopb.h"

namespace firebase {
namespace firestore {

bool operator<(const google_firestore_v1_ArrayValue&,
               const google_firestore_v1_ArrayValue&);
bool operator<(const google_firestore_v1_MapValue&,
               const google_firestore_v1_MapValue&);
bool operator<(const google_firestore_v1_MapValue_FieldsEntry&,
               const google_firestore_v1_MapValue_FieldsEntry&);
bool operator<(const google_firestore_v1_Value&,
               const google_firestore_v1_Value&);
bool operator<(const google_protobuf_Timestamp&,
               const google_protobuf_Timestamp&);
bool operator<(const google_type_LatLng&, const google_type_LatLng&);

}  // namespace firestore
}  // namespace firebase

#endif  // FIRESTORE_CORE_SRC_NANOPB_OPERATORS_H_
