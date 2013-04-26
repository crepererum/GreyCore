#ifndef GC_DTSTRING_HPP
#define GC_DTSTRING_HPP

#include <algorithm>
#include <array>
#include <string>

namespace greycore {
	template <std::size_t N>
	class DTString {
		public:
			DTString() : size(0) {}

			DTString(const std::string& s) :
					size(s.size()) {
				std::copy(s.cbegin(), s.cend(), data.begin());
			}

			DTString(const char* s) : DTString(std::string(s)) {}

			operator std::string() const {
				return std::string(data.cbegin(), data.cbegin() + size);
			}

			DTString& operator=(const DTString& obj) {
				size = obj.size;
				std::copy(obj.data.cbegin(), obj.data.cbegin() + size, data.begin());
				return *this;
			}

		private:
			std::size_t size;
			std::array<char, N> data;
	};

	template <std::size_t N>
	std::ostream& operator<<(std::ostream& stream, const DTString<N>& obj) {
		return stream << '"' << static_cast<const std::string&>(obj) << '"';
	}
}

#endif

