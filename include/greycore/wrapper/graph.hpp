#ifndef GC_GRAPH_HPP
#define GC_GRAPH_HPP

#include "../dim.hpp"

#include <list>
#include <memory>

namespace greycore {
	class Graph {
		public:
			typedef Dim<std::size_t> dimIndex_t;
			typedef Dim<std::size_t> dimData_t;

			Graph(const std::shared_ptr<dimIndex_t>& dimIndex, const std::shared_ptr<dimData_t>& dimData);

			std::size_t getSize() const;

			std::size_t add(const std::list<std::size_t>& refs);
			std::list<std::size_t> get(std::size_t id) const;

		private:
			std::shared_ptr<dimIndex_t> index;
			std::shared_ptr<dimData_t>  data;
	};
}

#endif

