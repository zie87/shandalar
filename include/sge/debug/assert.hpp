#ifndef SGE_DEBUG_ASSERT_HPP
#define SGE_DEBUG_ASSERT_HPP

#include <cassert>

#define SGE_CONTRACT_CHECK(str, x) assert(str && (x))

#define sge_expects(x) SGE_CONTRACT_CHECK("SGE: Precondition failure", x)
#define sge_ensures(x) SGE_CONTRACT_CHECK("SGE: Postcondition failure", x)

#endif // SGE_DEBUG_ASSERT_HPP
