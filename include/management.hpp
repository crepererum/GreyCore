#ifndef CG_MANAGEMENT_HPP
#define CG_MANAGEMENT_HPP

#include "config.hpp"
#include "dim.hpp"
#include "datatypes/dtstring.hpp"

#include <memory>

namespace greycore {
	typedef Dim<DTString<colnameLength>> indexColumns_t;

	std::shared_ptr<indexColumns_t> getIndexColumns(std::shared_ptr<DBFile> db);
}

#endif

