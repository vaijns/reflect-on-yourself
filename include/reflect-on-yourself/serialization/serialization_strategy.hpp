#ifndef H_REFLECT_ON_YOURSELF_SERIALIZATION_SERIALIZATION_STRATEGY
#define H_REFLECT_ON_YOURSELF_SERIALIZATION_SERIALIZATION_STRATEGY

#include "../reflection.hpp"

namespace roy::serialization{
	enum struct serialization_strategy_kind{
		as_object,
		as_array,
		custom
	};

	namespace strategy{
		inline constexpr roy::serialization::serialization_strategy_kind as_object{
			roy::serialization::serialization_strategy_kind::as_object
		};
		inline constexpr roy::serialization::serialization_strategy_kind as_array{
			roy::serialization::serialization_strategy_kind::as_array
		};
		inline constexpr roy::serialization::serialization_strategy_kind custom{
			roy::serialization::serialization_strategy_kind::custom
		};
	}
}

namespace roy{
	template<typename T, roy::serialization::serialization_strategy_kind Strategy>
	concept serialization_strategy = 
		roy::reflectable<T> and
		not roy::builtin<T> and
		roy::has_annotation<roy::serialization::serialization_strategy_kind, T>() and
		(roy::annotation_of<roy::serialization::serialization_strategy_kind, T>() == Strategy);
}

#endif
