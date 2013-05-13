#ifndef GC_FLATMAP_HPP
#define GC_FLATMAP_HPP

#include "dim.hpp"
#include "datatypes/dttuple.hpp"

#include <memory>
#include <unordered_map>

namespace greycore {
	template <typename K, typename V>
	class Flatmap {
		public:
			typedef DTTuple<K, V> payload_t;
			typedef Dim<payload_t> dim_t;

			Flatmap(const std::shared_ptr<dim_t>& dim_) :
					dim(dim_) {
				for (std::size_t s = 0; s < dim->getSegmentCount(); ++s) {
					typename dim_t::segment_t* segment = dim->getSegment(s);
					for (std::size_t i = 0; i < dim->getSegmentFillSize(s); ++i) {
						K key;
						V value;
						std::tie(key, value) = static_cast<std::tuple<K,V>>((*segment)[i]);
						map[key] = value;
					}
				}
			}

			const V& get(const K& key) const {
				return map.at(key);
			}

			void add(const K& key, const V& value) {
				map[key] = value;
				dim->add(payload_t(key, value));
			}

		private:
			std::shared_ptr<dim_t> dim;
			std::unordered_map<K, V> map;
	};
}

#endif

