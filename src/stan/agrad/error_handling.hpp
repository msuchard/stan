#ifndef __STAN__AGRAD__ERROR_HANDLING_HPP__
#define __STAN__AGRAD__ERROR_HANDLING_HPP__

#include <stan/math/error_handling.hpp>
#include <stan/agrad/agrad.hpp>

namespace boost {

  namespace math {

    /**
     * Categorizes the given stan::agrad::var value.
     * 
     * Categorizes the stan::agrad::var value, v, into the following categories:
     * zero, subnormal, normal, infinite, or NAN.
     *
     * @param v Variable to classify.
     * @return One of <code>FP_ZERO</code>, <code>FP_NORMAL</code>, 
     *   <code>FP_FINITE</code>, <code>FP_INFINITE</code>, <code>FP_NAN</code>,
     *   or <code>FP_SUBZERO</code>, specifying the category of v.
     */
    template <>
    inline
    int fpclassify(const stan::agrad::var v) {
      return boost::math::fpclassify(v.val());
    }

    /**
     * Checks if the given number has finite value.
     *
     * Return <code>true</code> if the specified variable's
     * value is finite.
     *
     * @param v Variable to test.
     * @return <code>true</code> if variable is finite.
     */
    template <>
    inline
    bool isfinite(const stan::agrad::var v) {
      return boost::math::isfinite(v.val());
    }

    /**
     * Checks if the given number is infinite.
     *
     * Return <code>true</code> if the specified variable's
     * value is infinite.
     *
     * @param v Variable to test.
     * @return <code>true</code> if variable is infinite.
     */
    template <>
    inline
    bool isinf(const stan::agrad::var v) {
      return boost::math::isinf(v.val());
    }

    /**
     * Checks if the given number is NaN
     *
     * Return <code>true</code> if the specified variable
     * has a value that is NaN.
     *
     * @param v Variable to test.
     * @return <code>true</code> if variable is NaN.
     */
    template <>
    inline
    bool isnan(const stan::agrad::var v) {
      return boost::math::isnan(v.val());
    }

    /**
     * Checks if the given number is normal.
     *
     * Return <code>true</code> if the specified variable
     * has a value that is normal.
     *
     * @param v Variable to test.
     * @return <code>true</code> if variable is normal.
     */
    template <>
    inline
    bool isnormal(const stan::agrad::var v) {
      return boost::math::isnormal(v.val());
    }

  }
}
  
#endif

