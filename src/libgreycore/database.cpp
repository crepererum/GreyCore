#include "greycore/database.hpp"

namespace greycore {
	Database::Database(const std::string& fname) :
			db(std::make_shared<DBFile>(fname)),
			indexDims(std::make_shared<indexDims_t>(db, "_indexdims")) {}

	std::shared_ptr<const Database::indexDims_t> Database::getIndexDims() const {
		return indexDims;
	}

	bool Database::dimExists(const std::string& name) const {
		return Dim<int>::exists(db, name);
	}
}

