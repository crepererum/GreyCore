#ifndef GC_DTSTRING_HPP
#define GC_DTSTRING_HPP

#include <algorithm>
#include <array>
#include <string>

namespace greycore {
	template <std::size_t N>
	class DTString {
		public:
			DTString(const std::string& s) :
					size(s.size()) {
				std::copy(s.cbegin(), s.cend(), data.begin());
			}

			DTString(const char* s) : DTString(std::string(s)) {}

			DTString() = default;
			DTString(const DTString&) = default;
			DTString(DTString&&) = default;
			DTString& operator=(const DTString&) = default;
			DTString& operator=(DTString&&) = default;

			operator std::string() const {
				return std::string(data.cbegin(), data.cbegin() + std::min(size, N));
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

