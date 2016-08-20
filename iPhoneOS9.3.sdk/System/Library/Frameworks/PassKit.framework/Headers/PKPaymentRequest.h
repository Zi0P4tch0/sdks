//
//  PKPaymentRequest.h
//
//  Copyright (c) 2014, Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AddressBook/ABRecord.h>

@class PKContact;

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, PKMerchantCapability) {
    PKMerchantCapability3DS                                 = 1UL << 0,   // Merchant supports 3DS
    PKMerchantCapabilityEMV                                 = 1UL << 1,   // Merchant supports EMV
    PKMerchantCapabilityCredit NS_ENUM_AVAILABLE_IOS(9_0)   = 1UL << 2,   // Merchant supports credit
    PKMerchantCapabilityDebit  NS_ENUM_AVAILABLE_IOS(9_0)   = 1UL << 3    // Merchant supports debit
} NS_ENUM_AVAILABLE(NA, 8_0);

typedef NS_OPTIONS(NSUInteger, PKAddressField) {
    PKAddressFieldNone                              = 0UL,      // No address fields required.
    PKAddressFieldPostalAddress                     = 1UL << 0, // Full street address including name, street, city, state/province, postal code, country.
    PKAddressFieldPhone                             = 1UL << 1,
    PKAddressFieldEmail                             = 1UL << 2,
    PKAddressFieldName NS_ENUM_AVAILABLE_IOS(8_3)   = 1UL << 3,
    PKAddressFieldAll                               = (PKAddressFieldPostalAddress|PKAddressFieldPhone|PKAddressFieldEmail|PKAddressFieldName)
} NS_ENUM_AVAILABLE(NA, 8_0);

typedef NS_ENUM(NSUInteger, PKShippingType) {
    PKShippingTypeShipping,
    PKShippingTypeDelivery,
    PKShippingTypeStorePickup,
    PKShippingTypeServicePickup
} NS_ENUM_AVAILABLE(NA, 8_3);

typedef NS_ENUM(NSUInteger, PKPaymentSummaryItemType) {
    PKPaymentSummaryItemTypeFinal,      // The payment summary item's amount is known to be correct
    PKPaymentSummaryItemTypePending     // The payment summary item's amount is estimated or unknown - e.g, a taxi fare
} NS_ENUM_AVAILABLE(NA, 9_0);

// PKPaymentSummaryItem Defines a line-item for a payment such as tax, shipping, or discount.
NS_CLASS_AVAILABLE(NA, 8_0)
@interface PKPaymentSummaryItem : NSObject

+ (instancetype)summaryItemWithLabel:(NSString *)label amount:(NSDecimalNumber *)amount;
+ (instancetype)summaryItemWithLabel:(NSString *)label amount:(NSDecimalNumber *)amount type:(PKPaymentSummaryItemType)type NS_AVAILABLE(NA, 9_0);

// A short localized description of the item, e.g. "Tax" or "Gift Card".
@property (nonatomic, copy) NSString *label;

// Same currency as the enclosing PKPaymentRequest.  Negative values are permitted, for example when
// redeeming a coupon. An amount is always required unless the summary item's type is set to pending
@property (nonatomic, copy) NSDecimalNumber *amount;

// Defaults to PKPaymentSummaryItemTypeFinal
// Set to PKPaymentSummaryItemTypePending if the amount of the item is not known at this time
@property (nonatomic, assign) PKPaymentSummaryItemType type NS_AVAILABLE(NA, 9_0);

@end

// Defines a shipping method for delivering physical goods.
NS_CLASS_AVAILABLE(NA, 8_0)
@interface PKShippingMethod : PKPaymentSummaryItem

// Application-defined unique identifier for this shipping method.  The application will receive this
// in paymentAuthorizationViewController:didAuthorizePayment:completion:.
@property (nonatomic, copy, nullable) NSString *identifier;

// Additional localized information about the shipping method, e.g. "Ships in 24 hours" or
// "Arrives Friday April 4."
@property (nonatomic, copy, nullable) NSString *detail;

@end

// PKPaymentRequest defines an application's request to produce a payment instrument for the
// purchase of goods and services. It encapsulates information about the selling party's payment
// processing capabilities, an amount to pay, and the currency code.
NS_CLASS_AVAILABLE(NA, 8_0)
@interface PKPaymentRequest : NSObject

// Identifies the merchant, as previously agreed with Apple.  Must match one of the merchant
// identifiers in the application's entitlement.
@property (nonatomic, copy) NSString *merchantIdentifier;

// The merchant's ISO country code.
@property (nonatomic, copy) NSString *countryCode;

// The payment networks supported by the merchant, for example @[ PKPaymentNetworkVisa,
// PKPaymentNetworkMasterCard ].  This property constrains payment cards that may fund the payment.
@property (nonatomic, copy) NSArray<NSString *> *supportedNetworks;

// The payment processing capabilities of the merchant.
@property (nonatomic, assign) PKMerchantCapability merchantCapabilities;

// Array of PKPaymentSummaryItem objects which should be presented to the user.
// The last item should be the total you wish to charge, and should not be pending
@property (nonatomic, copy) NSArray<PKPaymentSummaryItem *> *paymentSummaryItems;

// Currency code for this payment.
@property (nonatomic, copy) NSString *currencyCode;

// Indicates which billing address fields the merchant requires in order to process a transaction.
// The default is PKAddressFieldNone.
@property (nonatomic, assign) PKAddressField requiredBillingAddressFields;

// If the merchant already has a billing address on file, set it here.
@property (nonatomic, assign, nullable) ABRecordRef billingAddress NS_DEPRECATED_IOS(8_0, 9_0, "Use billingContact instead");
@property (nonatomic, retain, nullable) PKContact *billingContact NS_AVAILABLE_IOS(9_0);

// Indicates which shipping address fields the merchant requires in order to process a transaction.
// The default is PKAddressFieldNone.
@property (nonatomic, assign) PKAddressField requiredShippingAddressFields;

// If the merchant already has a shipping address on file, set it here.
@property (nonatomic, assign, nullable) ABRecordRef shippingAddress NS_DEPRECATED_IOS(8_0, 9_0, "Use shippingContact instead");
@property (nonatomic, retain, nullable) PKContact *shippingContact NS_AVAILABLE_IOS(9_0);

// Shipping methods supported by the merchant.
@property (nonatomic, copy, nullable) NSArray<PKShippingMethod *> *shippingMethods;

// Indicates the display mode for the shipping (e.g, "Pick Up", "Ship To", "Deliver To"). Localized.
// The default is PKShippingTypeShipping
@property (nonatomic, assign) PKShippingType shippingType NS_AVAILABLE_IOS(8_3);

// Optional merchant-supplied information about the payment request.  Examples of this are an order
// or cart identifier.  It will be signed and included in the resulting PKPaymentToken.
@property (nonatomic, copy, nullable) NSData *applicationData;

@end

NS_ASSUME_NONNULL_END