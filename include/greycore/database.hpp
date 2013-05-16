#ifndef GC_DATABASE_HPP
#define GC_DATABASE_HPP

#include "config.hpp"
#include "dbfile.hpp"
#include "dim.hpp"
#include "datatypes/dttuple.hpp"
#include "datatypes/dtstring.hpp"

#include <memory>
#include <stdexcept>
#include <string>

namespace greycore {
	class Database {
		public:
			typedef DTTuple<DTString<colnameLength>, DTString<typenameLength>> indexDimsPayload_t;
			typedef Dim<indexDimsPayload_t> indexDims_t;

			Database(const std::string& fname);

			std::shared_ptr<const indexDims_t> getIndexDims() const;

			bool dimExists(const std::string& name) const;

			template <typename T>
			std::shared_ptr<Dim<T>> createDim(const std::string& name) {
				if ((name.size() < 1) || (name[0] == '_')) {
					throw std::runtime_error("Illegal name!");
				}

				if (!dimExists(name)) {
					auto tmp = std::make_shared<Dim<T>>(db, name);
					indexDims->add(indexDimsPayload_t(tmp->getName(), tmp->getTypename()));
					return tmp;
				} else {
					throw std::runtime_error("Dimension already exists!");
				}
			}

			template <typename T>
			std::shared_ptr<Dim<T>> getDim(const std::string& name) {
				if (dimExists(name)) {
					return std::make_shared<Dim<T>>(db, name);
				} else {
					throw std::runtime_error("Dimension does not exist!");
				}
			}

		private:
			std::shared_ptr<DBFile> db;
			std::shared_ptr<indexDims_t> indexDims;
	};
}

#endif

