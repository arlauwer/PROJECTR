#pragma once

#include <gtest/gtest.h>

template <typename T>
::testing::AssertionResult
ExpectNearRelative(const char* a_expr, const char* b_expr, const char* rel_tol_expr, T a, T b, T rel_tol)
{
    T diff    = std::abs(a - b);
    T largest = std::max(std::abs(a), std::abs(b));

    if (diff <= largest * rel_tol)
    {
        return ::testing::AssertionSuccess();
    }

    return ::testing::AssertionFailure() << a_expr << " (" << a << ")\n"
                                         << b_expr << " (" << b << ")\ndiffer by " << diff
                                         << " which is greater than relative tolerance " << rel_tol_expr << " ("
                                         << largest * rel_tol << ")";
}

#define EXPECT_NEAR_REL(a, b, rel_tol) EXPECT_PRED_FORMAT3(ExpectNearRelative, a, b, rel_tol)