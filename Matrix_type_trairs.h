#pragma once
#include <vector>

template <size_t N, size_t M, typename T>
class Matrix;

template <typename E1, typename E2>
class Matrix_sum;

template <typename E1, typename E2>
class Matrix_multiply;

template <typename T>
class Matrix_expression;

template <typename T, typename U>
class Matrix_pair;

template <typename... Tail>
struct Sizes {};

template <size_t N, size_t M, typename T, typename... Tail>
struct Sizes<Matrix<N, M, T>, Tail...> {
	static const size_t value_N = N;
	static const size_t value_M = M;
};

template <size_t N, size_t M, typename T>
struct Sizes<Matrix<N, M, T>> {
	static const size_t value_N = N;
	static const size_t value_M = M;
};

template <typename E1, typename E2>
struct Sizes<Matrix_sum<E1, E2>> {
	static const size_t value_N = Sizes<E1>::value_N;
	static const size_t value_M = Sizes<E1>::value_M;
};

template <typename E1, typename E2>
struct Sizes<Matrix_multiply<E1, E2>> {
	static const size_t value_N = Sizes<E1>::value_N;
	static const size_t value_M = Sizes<E2>::value_M;
};

// GET_TYPE	
template <typename... Tail>
struct Get_type {};

template <size_t N, size_t M, typename T, typename... Tail>
struct Get_type<Matrix<N, M, T>, Tail...> {
	using type = T;
};

template <size_t N, size_t M, typename T>
struct Get_type<Matrix<N, M, T>> {
	using type = T;
};

template <typename T>
struct Get_type<std::vector<T>> {
	using type = T;
};

template <typename E1, typename E2>
struct Get_type<Matrix_pair<E1, E2>> {
	using type = std::common_type_t<typename Get_type<E1>::type, typename Get_type<E2>::type>;
};

template <typename E1, typename E2>
struct Get_type<Matrix_multiply<E1, E2>> {
	using type = std::common_type_t<typename Get_type<E1>::type, typename Get_type<E2>::type>;
};

template <typename... Tail>
using get_type_t = typename Get_type<Tail...>::type;

// RETURN_TYPE
template <typename T>
struct Return_type {};

template <size_t N, size_t M, typename T>
struct Return_type<Matrix<N, M, T>> {
	using type = std::vector<T>;
};

template <typename E1, typename E2>
struct Return_type<Matrix_sum<E1, E2>> {
	using type = Matrix_pair<typename Return_type<E1>::type,
		typename Return_type<E2>::type>;
};

template <typename E1, typename E2>
struct Return_type<Matrix_multiply<E1, E2>> {
	using type = const std::vector<std::common_type_t<get_type_t<E1>, get_type_t<E2>>>;
};

template <typename T>
using return_type_t = typename Return_type<T>::type;

// GET_MATRIX
template <typename T>
struct Get_matrix {};

template <size_t N, size_t M, typename T>
struct Get_matrix<Matrix<N, M, T>> {
private:
	const Matrix<N, M, T>& data;
public:
	Get_matrix(const Matrix<N, M, T>&);
	const Matrix<N, M, T>& get() const;
};

template <typename E1, typename E2>
struct Get_matrix<Matrix_multiply<E1, E2>> {
private:
	Matrix<Sizes<E1>::value_N, Sizes<E2>::value_M,
		std::common_type_t<get_type_t<E1>, get_type_t<E2>>> data;
public:
	Get_matrix(const Matrix_multiply<E1, E2>&);
	auto get() const ->
		Matrix<Sizes<E1>::value_N, Sizes<E2>::value_M,
		std::common_type_t<get_type_t<E1>, get_type_t<E2>>>;
};

// VERIFY_DIMENSIONS
template <typename E1, typename E2>
struct Verify_dimension {
	static constexpr bool value = (Sizes<E1>::value_N == Sizes<E2>::value_N) &&
		(Sizes<E1>::value_M == Sizes<E2>::value_M);
};

template <typename E1, typename E2>
struct Verify_dimension_for_multiply {
	static constexpr bool value = (Sizes<E1>::value_M == Sizes<E2>::value_N);
};

// IS_VECTOR
template <typename T>
struct Is_vector : std::false_type {};

template <typename T>
struct Is_vector<std::vector<T>> : std::true_type {};

template <typename T>
const bool is_vector_v = Is_vector<T>::value;

// VERIFY_SIZE
template <typename... Tail>
struct Verify_size_for_add {};

template <size_t N, size_t M, typename T, typename... Tail>
struct Verify_size_for_add<Matrix<N, M, T>, Tail...> {
	static const bool value =
		((N == Sizes<Tail>::value_N && M == Sizes<Tail>::value_M) && ...);
};

template <size_t N, size_t M, typename T>
struct Verify_size_for_add<Matrix<N, M, T>> {
	static const bool value = true;
};

// COMMON_TYPE
template <typename... Tail>
struct Helper_Common_type {};

template <typename T, typename U>
struct Helper_Common_type<T, U> {
	using type = std::common_type_t<get_type_t<T>, get_type_t<U>>;
};

template <typename T, typename U, typename... Tail>
struct Helper_Common_type<T, U, Tail...> {
	using type = std::common_type_t <get_type_t<T>, get_type_t<U>,
		typename Helper_Common_type<U, Tail...>::type>;
};

template <typename T>
struct Helper_Common_type<T> {
	using type = get_type_t<T>;
};

template <typename... Tail>
struct Common_type {
	using type = typename Helper_Common_type<Tail...>::type;
};

