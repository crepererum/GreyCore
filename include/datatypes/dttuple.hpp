#ifndef GC_DTTUPLE_HPP
#define GC_DTTUPLE_HPP

#include <ostream>
#include <tuple>
#include <utility>

namespace greycore {
	template <typename Head, typename... Tail>
	class DTTuple {
		public:
			DTTuple(Head h, Tail... t) : head(h), tail(t...) {}
			//DTTuple(const Head& h, const Tail&... t) : head(h), tail(t...) {}
			//DTTuple(Head&& h, Tail&&... t) : head(std::forward(h)), tail(std::forward(t)...) {}

			DTTuple() = default;
			DTTuple(const DTTuple&) = default;
			DTTuple(DTTuple&&) = default;
			DTTuple& operator=(const DTTuple&) = default;
			DTTuple& operator=(DTTuple&&) = default;

			operator std::tuple<Head, Tail...>() const {
				return std::tuple_cat(std::tuple<Head>(head), static_cast<std::tuple<Tail...>>(tail));
			}

		private:
			Head head;
			DTTuple<Tail...> tail;
	};

	template <typename Head>
	class DTTuple<Head> {
		public:
			DTTuple(Head h) : head(h) {}
			//DTTuple(const Head& h) : head(h) {}
			//DTTuple(Head&& h ) : head(std::forward(h)) {}

			DTTuple() = default;
			DTTuple(const DTTuple&) = default;
			DTTuple(DTTuple&&) = default;
			DTTuple& operator=(const DTTuple&) = default;
			DTTuple& operator=(DTTuple&&) = default;

			operator std::tuple<Head>() const {
				return std::tuple<Head>(head);
			}

		private:
			Head head;
	};

	// print helper class
	template<std::size_t> struct int_{};

	template <class Tuple, size_t Pos>
	std::ostream& print_tuple(std::ostream& stream, const Tuple& t, int_<Pos> ) {
		stream << std::get<std::tuple_size<Tuple>::value - Pos>(t) << " | ";
		return print_tuple(stream, t, int_<Pos - 1>());
	}

	template <class Tuple>
	std::ostream& print_tuple(std::ostream& stream, const Tuple& t, int_<1> ) {
		return stream << std::get<std::tuple_size<Tuple>::value - 1>(t);
	}

	template <class... Args>
	std::ostream& operator<<(std::ostream& stream, const std::tuple<Args...>& t) {
		return print_tuple(stream, t, int_<sizeof...(Args)>());
	}

	template <typename Head, typename... Tail>
	std::ostream& operator<<(std::ostream& stream, const DTTuple<Head, Tail...>& t) {
		return stream << static_cast<const std::tuple<Head, Tail...>&>(t);
	}
}

#endif

