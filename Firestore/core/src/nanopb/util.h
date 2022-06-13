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

#ifndef FIRESTORE_CORE_SRC_NANOPB_UTIL_H_
#define FIRESTORE_CORE_SRC_NANOPB_UTIL_H_

#include <vector>

#include "Firestore/Protos/nanopb/google/firestore/v1/document.nanopb.h"

namespace firebase {
namespace firestore {
namespace nanopb {

/**
 * Creates a sorted vector with the field entries from the given MapValue.
 *
 * Each element of the returned vector is a distinct pointer from the `fields`
 * member of the given `google_firestore_v1_MapValue` object.
 *
 * The elements are sorted by the objects pointed to by the elements. The sort
 * order is undefined; however, it is consistent and is, therefore, useful when
 * comparing two `google_firestore_v1_MapValue` objects where order of their
 * fields is not important (e.g. AbslHashValue() and operator<).
 */
std::vector<google_firestore_v1_MapValue_FieldsEntry*> SortedEntries(
    const google_firestore_v1_MapValue&);

}  // namespace nanopb
}  // namespace firestore
}  // namespace firebase

#endif  // FIRESTORE_CORE_SRC_NANOPB_UTIL_H_
