#ifndef __STAN__PROB__DISTRIBUTIONS__MULTIVARIATE__CONTINUOUS__DIRICHLET_HPP__
#define __STAN__PROB__DISTRIBUTIONS__MULTIVARIATE__CONTINUOUS__DIRICHLET_HPP__

#include <boost/random/gamma_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <stan/prob/constants.hpp>
#include <stan/math/matrix_error_handling.hpp>
#include <stan/math/error_handling.hpp>
#include <stan/math/special_functions.hpp>
#include <stan/prob/traits.hpp>

namespace stan {

  namespace prob {

    /**
     * The log of the Dirichlet density for the given theta and
     * a vector of prior sample sizes, alpha.
     * Each element of alpha must be greater than 0. 
     * Each element of theta must be greater than or 0.
     * Theta sums to 1.
     *
     * \f{eqnarray*}{
       \theta &\sim& \mbox{\sf{Dirichlet}} (\alpha_1, \ldots, \alpha_k) \\
       \log (p (\theta \,|\, \alpha_1, \ldots, \alpha_k) ) &=& \log \left( \frac{\Gamma(\alpha_1 + \cdots + \alpha_k)}{\Gamma(\alpha_1) \cdots \Gamma(\alpha_k)}
          \theta_1^{\alpha_1 - 1} \cdots \theta_k^{\alpha_k - 1} \right) \\
       &=& \log (\Gamma(\alpha_1 + \cdots + \alpha_k)) - \log(\Gamma(\alpha_1)) - \cdots - \log(\Gamma(\alpha_k)) +
           (\alpha_1 - 1) \log (\theta_1) + \cdots + (\alpha_k - 1) \log (\theta_k)
     \f}
     * 
     * @param theta A scalar vector.
     * @param alpha Prior sample sizes.
     * @return The log of the Dirichlet density.
     * @throw std::domain_error if any element of alpha is less than
     * or equal to 0.
     * @throw std::domain_error if any element of theta is less than 0.
     * @throw std::domain_error if the sum of theta is not 1.
     * @tparam T_prob Type of scalar.
     * @tparam T_prior_sample_size Type of prior sample sizes.
     */
    template <bool propto,
              typename T_prob, typename T_prior_sample_size, 
              class Policy>
    typename boost::math::tools::promote_args<T_prob,T_prior_sample_size>::type
    dirichlet_log(const Eigen::Matrix<T_prob,Eigen::Dynamic,1>& theta,
              const Eigen::Matrix<T_prior_sample_size,Eigen::Dynamic,1>& alpha,
              const Policy&) {
      // FIXME: parameter check
      using boost::math::tools::promote_args;
      typename promote_args<T_prob,T_prior_sample_size>::type lp(0.0);

      using stan::math::multiply_log;
      if (include_summand<propto,T_prior_sample_size>::value) {
        lp += lgamma(alpha.sum());
        for (int k = 0; k < alpha.rows(); ++k)
          lp -= lgamma(alpha[k]);
      }
      if (include_summand<propto,T_prob,T_prior_sample_size>::value)
        for (int k = 0; k < theta.rows(); ++k) 
          lp += multiply_log(alpha[k]-1, theta[k]);
      return lp;
    }

    template <bool propto,
              typename T_prob, typename T_prior_sample_size>
    inline
    typename boost::math::tools::promote_args<T_prob,T_prior_sample_size>::type
    dirichlet_log(const Eigen::Matrix<T_prob,Eigen::Dynamic,1>& theta,
          const Eigen::Matrix<T_prior_sample_size,Eigen::Dynamic,1>& alpha) {
      return dirichlet_log<propto>(theta,alpha,stan::math::default_policy());
    }


    template <typename T_prob, typename T_prior_sample_size, 
              class Policy>
    inline
    typename boost::math::tools::promote_args<T_prob,T_prior_sample_size>::type
    dirichlet_log(const Eigen::Matrix<T_prob,Eigen::Dynamic,1>& theta,
              const Eigen::Matrix<T_prior_sample_size,Eigen::Dynamic,1>& alpha,
              const Policy&) {
      return dirichlet_log<false>(theta,alpha,Policy());
    }

    template <typename T_prob, typename T_prior_sample_size>
    inline
    typename boost::math::tools::promote_args<T_prob,T_prior_sample_size>::type
    dirichlet_log(const Eigen::Matrix<T_prob,Eigen::Dynamic,1>& theta,
            const Eigen::Matrix<T_prior_sample_size,Eigen::Dynamic,1>& alpha) {
      return dirichlet_log<false>(theta,alpha,stan::math::default_policy());
    }

    template <class RNG>
    inline Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>
    dirichlet_rng(const Eigen::Matrix<double,Eigen::Dynamic,1>& alpha,
                     RNG& rng) {
      using boost::variate_generator;
      using boost::gamma_distribution;

      double sum = 0;
      Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> y(alpha.rows(), 1);
      for(int i = 0; i < alpha.rows(); i++)
	{
        variate_generator<RNG&, gamma_distribution<> >
	  gamma_rng(rng, gamma_distribution<>(alpha(i,0),1));
	y(i,0) = gamma_rng();
	sum += y(i,0);
	}

      for(int i = 0; i < alpha.rows(); i++)
	y(i,0) /= sum;
      return y;
    }
  }
}
#endif
