#ifndef VectorUtil_h
#define VectorUtil_h

#include <vector>
#include <functional>
#include <algorithm>

template<typename TFrom, typename TTo>
std::vector<TTo> Collect(
      const std::vector<TFrom>& source,
      std::function<TTo(TFrom)> func) {
   std::vector<TTo> result;
   std::transform(source.begin(), source.end(), std::back_inserter(result), func);
   return result;
}

#endif
