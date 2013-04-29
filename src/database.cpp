#include "database.hpp"

namespace greycore {
	Database::Database(std::string fname) :
			db(std::make_shared<DBFile>(fname)),
			indexDims(std::make_shared<indexDims_t>(db, "_indexdims")) {}

	std::shared_ptr<const Database::indexDims_t> Database::getIndexDims() const {
		return indexDims;
	}

	bool Database::dimExists(std::string name) const {
		return Dim<int>::exists(db, name);
	}
}

