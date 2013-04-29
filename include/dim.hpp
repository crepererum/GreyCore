#ifndef GC_DIM_HPP
#define GC_DIM_HPP

#include "config.hpp"
#include "dbfile.hpp"

#include <array>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>

#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

namespace greycore {
	template <typename T>
	class Dim {
		static_assert(std::is_pod<T>::value, "Dim<A> can only hold POD");

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

			std::string getName() const {
				return this->name;
			}

			std::size_t getSize() const {
				return *this->size;
			}

			std::size_t add(T x) {
				std::size_t pos = *this->size;
				std::size_t segment = pos / SEGMENT_SIZE;
				std::size_t offset = pos % SEGMENT_SIZE;
				segment_t* segmentPtr = getSegmentPtr(segment);

				// store data
				(*segmentPtr)[offset] = x;
				(*this->size)++;

				return pos;
			}

			segment_t* getSegment(std::size_t segment) {
				return getSegmentPtr(segment);
			}

			const segment_t* getSegment(std::size_t segment) const {
				return getSegmentPtr(segment);
			}

			std::size_t getSegmentCount() const {
				if ((*this->size) > 0) {
					return (*this->size - 1) / SEGMENT_SIZE + 1;
				} else {
					return 0;
				}
			}

			std::size_t getSegmentFillSize(std::size_t segment) const {
				if ((segment + 1) * SEGMENT_SIZE <= (*this->size)) {
					// full
					return SEGMENT_SIZE;
				} else {
					// partial
					return (*this->size) % SEGMENT_SIZE;
				}
			}

			static bool exists(std::shared_ptr<DBFile> file, std::string name) {
				return file->find<std::size_t>(genSizeID(name)).second == 1;
			}

		private:
			mutable std::mutex mutexSegments;

			std::string name;
			std::shared_ptr<DBFile> file;
			std::size_t* size;
			mutable std::unordered_map<std::size_t, segment_t*> segments;
			int callbackID;

			static std::string genSizeID(std::string name) {
				std::stringstream buffer;
				buffer << "dims/" << name << "/size";
				return buffer.str();
			}

			static std::string genSegmentID(std::string name, std::size_t n) {
				std::stringstream buffer;
				buffer << "dims/" << name << "/segments/" << n;
				return buffer.str();
			}

			void resetPtrs() {
				this->size = file->find_or_construct<std::size_t>(genSizeID(name), 0);

				// reset segment pointers
				this->segments.clear();
			}

			segment_t* getSegmentPtr(std::size_t segment) const {
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

	template <typename T>
	std::ostream& operator<<(std::ostream& stream, const Dim<T>& obj) {
		std::string name(obj.getName());
		stream << name << std::endl;
		stream << std::string(name.size(), '-') << std::endl << std::endl;

		for (std::size_t s = 0; s < obj.getSegmentCount(); ++s) {
			auto segment = obj.getSegment(s);

			for (std::size_t i = 0; i < obj.getSegmentFillSize(s); ++i) {
				stream << (*segment)[i] << std::endl;
			}
		}

		return stream;
	}
}

#endif

