#import <Foundation/Foundation.h>

@class FIRDocumentReference;

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(DirectToServerQueryExecutor)
@interface FIRDirectToServerQueryExecutor : NSObject

/** :nodoc: */
- (id)init __attribute__((unavailable("FIRDirectToServerQueryExecutor  cannot be created directly.")));

+ (FIRDirectToServerQueryExecutor *)DirectToServerQueryExecutorForDocument:(FIRDocumentReference *)doc;

@end

NS_ASSUME_NONNULL_END

