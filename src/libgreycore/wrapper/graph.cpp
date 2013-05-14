#include "greycore/wrapper/graph.hpp"

#include <cassert>
#include <sstream>

namespace greycore {
	Graph::Graph(const std::shared_ptr<dimIndex_t>& dimIndex, const std::shared_ptr<dimData_t>& dimData) :
			index(dimIndex),
			data(dimData) {
		if (index->getSize() == 0) {
			assert(data->getSize() == 0);
			index->add(0);
		}
	}

	std::size_t Graph::getSize() const {
		return index->getSize() - 1;
	}

	std::size_t Graph::add(const std::list<std::size_t>& refs) {
		data->add(refs.begin(), refs.end());
		index->add(data->getSize());

		return index->getSize() - 2;
	}

	std::list<std::size_t> Graph::get(std::size_t id) const {
		std::size_t begin = (*index)[id];
		std::size_t end = (*index)[id + 1];
		std::list<std::size_t> result;

		for (; begin < end; ++begin) {
			result.push_back((*data)[begin]);
		}

		return result;
	}
}

