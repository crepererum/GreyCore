#ifndef GC_DATABASE_HPP
#define GC_DATABASE_HPP

#include "config.hpp"
#include "dbfile.hpp"
#include "dim.hpp"
#include "datatypes/dtstring.hpp"

#include <memory>
#include <stdexcept>
#include <string>

namespace greycore {
	class Database {
		public:
			typedef Dim<DTString<colnameLength>> indexDims_t;

			Database(std::string fname);

			std::shared_ptr<const indexDims_t> getIndexDims() const;

			bool dimExists(std::string name) const;

			template <typename T>
			std::shared_ptr<Dim<T>> createDim(std::string name) {
				if ((name.size() < 1) || (name[0] == '_')) {
					throw new std::runtime_error("Illegal name!");
				}

				if (!dimExists(name)) {
					indexDims->add(name);
					return std::make_shared<Dim<T>>(db, name);
				} else {
					throw new std::runtime_error("Dimension already exists!");
				}
			}

			template <typename T>
			std::shared_ptr<Dim<T>> getDim(std::string name) {
				if (dimExists(name)) {
					return std::make_shared<Dim<T>>(db, name);
				} else {
					throw new std::runtime_error("Dimension does not exist!");
				}
			}

		private:
			std::shared_ptr<DBFile> db;
			std::shared_ptr<indexDims_t> indexDims;
	};
}

#endif

