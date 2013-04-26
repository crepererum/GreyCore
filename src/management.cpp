#include "management.hpp"

namespace greycore {
	std::shared_ptr<indexColumns_t> getIndexColumns(std::shared_ptr<DBFile> db) {
		return std::make_shared<indexColumns_t>(db, "_columns");
	}
}

