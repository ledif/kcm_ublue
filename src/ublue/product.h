#pragma once

#include <QString>

namespace ublue {

using namespace Qt::Literals::StringLiterals;

enum class ProductName
{
    unknown = 0,
    aurora = 1,
    bazzite = 2,
    bluefin = 3
};

template<ProductName Product>
struct ProductTraitsImpl;

template<>
struct ProductTraitsImpl<ProductName::aurora>
{
    static QString imageName() { return "aurora"_L1; }
};

template<>
struct ProductTraitsImpl<ProductName::bazzite>
{
    static QString imageName() { return "bazzite"_L1; }
};

using ProductTraits = ProductTraitsImpl<
#if defined(UBLUE_PRODUCT_AURORA)
    ProductName::aurora
#elif defined(UBLUE_PRODUCT_BAZZITE)
    ProductName::bazzite
#elif defined(UBLUE_PRODUCT_BLUEFIN)
    ProductName::bluefin
#else
#error "No ublue product name defined"
    ProductName::unknown
#endif
    >;

} // namespace ublue