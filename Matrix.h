#pragma once
#include <vector>
#include <ostream>
#include <type_traits>
#include <utility>
#include "Matrix_type_trairs.h"
#include "big_integer.h"

template <typename E1, typename E2>
class Matrix_pair {
private:
	std::pair<std::conditional_t<is_vector_v<E1>, const E1&, E1>,
		std::conditional_t<is_vector_v<E2>, const E2&, E1>> data;
public:
	Matrix_pair(const E1&, const E2&);
	auto operator[](size_t j) const ->
		std::common_type_t<get_type_t<E1>, get_type_t<E2>>;
};

template <typename E1, typename E2>
class Matrix_sum : public Matrix_expression<Matrix_sum<E1, E2>> {
public:
	static constexpr bool is_leaf = false;
private:
	std::conditional_t<E1::is_leaf, const E1&, E1> e1_;
	std::conditional_t<E2::is_leaf, const E2&, E2> e2_;

public:
	Matrix_sum(const E1&, const E2&);
public:
	auto operator[](size_t) const ->
		Matrix_pair<return_type_t<E1>, return_type_t<E2>>;
};

template <typename E1, typename E2>
class Matrix_multiply : public Matrix_expression<Matrix_multiply<E1, E2>> {
public:
	static constexpr bool is_leaf = false;
private:
	std::conditional_t<E1::is_leaf, const E1&, E1> e1_;
	std::conditional_t<E2::is_leaf, const E2&, E2> e2_;

public:
	Matrix_multiply(const E1&, const E2&);
public:
	auto operator[](size_t) const ->
		std::vector<std::common_type_t<get_type_t<E1>, get_type_t<E2>>>;

	template <typename U, typename E1, typename E2>
	friend auto operator* <>(const std::vector<U>& vec, const Matrix_multiply<E1, E2>& matr) ->
		std::vector<std::common_type_t<U, get_type_t<E1>, get_type_t<E2>>>;

	template <typename U>
	friend struct Get_matrix;
};

template <typename U, typename E1, typename E2>
auto operator*(const std::vector<U>& vec, const Matrix_multiply<E1, E2>& matr) ->
std::vector<std::common_type_t<U, get_type_t<E1>, get_type_t<E2>>>;

template <typename T>
class Matrix_expression {
public:
	static constexpr bool is_leaf = false;

	return_type_t<T> operator[](size_t) const;
};

template <size_t N, size_t M, typename T = int>
class Matrix : public Matrix_expression<Matrix<N, M, T>> {
public:
	static constexpr bool is_leaf = true;
private:
	std::vector<std::vector<T>> data;

public:
	Matrix();
	Matrix(const std::vector<std::vector<T>>&);
	Matrix(const Matrix<N, M, T>&);
	Matrix(size_t, size_t);

	template <typename U>
	Matrix<N, M, T>& operator=(const Matrix<N, M, U>&);

	std::vector<T>& operator[](size_t);
	const std::vector<T>& operator[](size_t) const;

	template <typename U>
	Matrix<N, M, T>& operator+=(const Matrix<N, M, U>&);

	template <typename U>
	Matrix<N, M, T>& operator-=(const Matrix<N, M, U>&);

	Matrix<N, M, T>& operator*=(int);
	Matrix<N, M, T>& operator*=(double);

	template <size_t K, typename U>
	Matrix<N, M, T>& operator*=(const Matrix<M, K, U>&);

	template <typename U>
	bool operator==(const Matrix<N, M, U>&) const;

	template <typename U>
	bool operator!=(const Matrix<N, M, U>&) const;

	const std::vector<std::vector<T>>& get_data() const;
	Matrix<N - 1, N - 1, T> minor(size_t, size_t) const;
	size_t size() const;
	size_t rank() const;
	Matrix<N, M, double> invert() const;
	double det() const;
	Matrix<M, N, T> transpouse() const;
	void swap_row(size_t, size_t, size_t);

	const std::vector<T>& get_row(size_t) const;
	std::vector<T> get_col(size_t) const;
	T trace() const;
};

template <size_t N, size_t M, typename T, typename U>
std::vector<std::common_type_t<T, U>> operator*(const std::vector<T>&, const Matrix<N, M, U>&);

template <size_t N, size_t M, size_t K, typename T, typename U>
auto operator*(const Matrix<N, M, T>& left, const Matrix<M, K, U>& right) ->
Matrix<N, K, std::common_type_t<T, U>>;

template <typename E1, typename E2>
Matrix_multiply<E1, E2> operator*(const Matrix_expression<E1>& m1, const Matrix_expression<E2>& m2);

template <typename E1, typename E2>
Matrix_sum<E1, E2> operator+(const Matrix_expression<E1>& m1, const Matrix_expression<E2>& m2);

template <typename U, typename E1, typename E2>
auto operator*(const std::vector<U>& vec, const Matrix_multiply<E1, E2>& matr) ->
std::vector<std::common_type_t<U, get_type_t<E1>, get_type_t<E2>>>;

template <size_t N, size_t M, typename T, typename U>
auto operator+(const Matrix<N, M, T>& left, const Matrix<N, M, U>& right) ->
Matrix<N, M, std::common_type_t<T, U>>;

template <size_t N, size_t M, size_t K, typename T, typename U>
auto operator*(const Matrix<N, M, T>& left, const Matrix<M, K, U>& right) ->
Matrix<N, K, std::common_type_t<T, U>>;

template <typename... Tail>
auto add(const Tail&... tail) ->
Matrix<Sizes<Tail...>::value_N, Sizes<Tail...>::value_M, typename Common_type<Tail...>::type>;

template <typename... Tail>
auto substract(const Tail&... tail) -> 
Matrix<Sizes<Tail...>::value_N, Sizes<Tail...>::value_M, typename Common_type<Tail...>::type>;

template <size_t N, typename T>
using Square_matrix = Matrix<N, N, T>;

template <typename T>
T ABS(const T& value);