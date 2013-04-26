#include "dbfile.hpp"

#include <cassert>

namespace bip = boost::interprocess;

namespace greycore {
	DBFile::DBFile(std::string fname) :
			name(fname),
			mfile(new mfile_t(bip::open_or_create, name.c_str(), GROW_SIZE)) {
		assert(mfile->check_sanity());
	}

	int DBFile::registerPResetFun(pResetFun_t fun) {
		int id = this->idCounter++;
		this->resetFuns[id] = fun;
		return id;
	}

	void DBFile::unregisterPResetFun(int id) {
		this->resetFuns.erase(id);
	}

	void DBFile::grow() {
		// unmap
		mfile_t* ptr = mfile.get();
		mfile.release();
		delete ptr;

		// grow
		bip::managed_mapped_file::grow(name.c_str(), GROW_SIZE);

		// remap
		ptr = new mfile_t(bip::open_only, name.c_str());
		mfile.reset(ptr);

		// reset all pointers
		for (auto f : this->resetFuns) {
			f.second();
		}
	}
}

