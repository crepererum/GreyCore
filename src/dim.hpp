#ifndef DIM_HPP
#define DIM_HPP

#include <array>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <sstream>
#include <string>
#include <unordered_map>

#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

#include "config.hpp"
#include "dbfile.hpp"

template <typename T>
class Dim {
	public:
		static constexpr unsigned int SEGMENT_SIZE = 64 * (pageSize - sizeof(std::array<T,0>)) / sizeof(T); // 64 pages (opt 256 pages)
		typedef std::array<T, SEGMENT_SIZE> segment_t;

		Dim(std::shared_ptr<DBFile> file, std::string name) :
				name(name),
				file(file),
				callbackID(file->registerPResetFun(std::bind(&Dim::resetPtrs, this))) {
			resetPtrs();
		}

		~Dim() {
			file->unregisterPResetFun(callbackID);
		}

		std::string getName() {
			return this->name;
		}

		long getSize() {
			return *this->size;
		}

		long add(T x) {
			long pos = *this->size;
			long segment = pos / SEGMENT_SIZE;
			long offset = pos % SEGMENT_SIZE;
			segment_t* segmentPtr = getSegmentPtr(segment);

			// store data
			(*segmentPtr)[offset] = x;
			(*this->size)++;

			return pos;
		}

		segment_t* getSegment(long segment) {
			return getSegmentPtr(segment);
		}

		long getSegmentCount() {
			if ((*this->size) > 0) {
				return (*this->size - 1) / SEGMENT_SIZE + 1;
			} else {
				return 0;
			}
		}

		long getSegmentFillSize(long segment) {
			if ((segment + 1) * SEGMENT_SIZE <= (*this->size)) {
				// full
				return SEGMENT_SIZE;
			} else {
				// partial
				return (*this->size) % SEGMENT_SIZE;
			}
		}

	private:
		std::mutex mutexSegments;

		std::string name;
		std::shared_ptr<DBFile> file;
		long* size;
		std::unordered_map<long, segment_t*> segments;
		int callbackID;

		static std::string genSizeID(std::string name) {
			std::stringstream buffer;
			buffer << "dims/" << name << "/size";
			return buffer.str();
		}

		static std::string genSegmentID(std::string name, long n) {
			std::stringstream buffer;
			buffer << "dims/" << name << "/segments/" << n;
			return buffer.str();
		}

		void resetPtrs() {
			this->size = file->find_or_construct<long>(genSizeID(name), 0);

			// reset segment pointers
			this->segments.clear();
		}

		segment_t* getSegmentPtr(long segment) {
			std::lock_guard<std::mutex> lock(this->mutexSegments);
			segment_t* segmentPtr = nullptr;

			// get segment
			try {
				segmentPtr = this->segments.at(segment);
			} catch (std::out_of_range& e) {
				// segment isn't loaded
				std::string sID = genSegmentID(name, segment);

				// try to load segement
				auto lookup = file->find<segment_t>(sID);
				if (lookup.second == 1) {
					// found
					segmentPtr = lookup.first;
				} else {
					// new segment
					segmentPtr = file->construct<segment_t>(sID);
				}

				this->segments.emplace(segment, segmentPtr);
			}

			return segmentPtr;
		}
};

#endif

