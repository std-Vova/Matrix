
#include<vector>
#include<string>
#include<iostream>

class rational;

class big_integer {
private:
	bool sign = true;
	std::vector<int8_t> v;
public:
	big_integer();
	big_integer(int);
	big_integer(unsigned);
	big_integer(long);
	big_integer(long long);
	big_integer(unsigned long);
	big_integer(unsigned long long);
	big_integer(const char*);
	explicit big_integer(const std::string&);
	big_integer(const std::vector<int>&);
	big_integer(const big_integer&);
	big_integer(double);
	big_integer& operator=(const big_integer&);

	friend std::ostream& operator<<(std::ostream&, const big_integer&);
	friend std::istream& operator>>(std::istream&, big_integer&);

	friend bool operator>(const big_integer&, const big_integer&);
	friend bool operator>=(const big_integer&, const big_integer&);

	friend bool operator<(const big_integer&, const big_integer&);
	friend bool operator<=(const big_integer&, const big_integer&);

	friend bool operator==(const big_integer&, const big_integer&);
	friend bool operator!=(const big_integer&, const big_integer&);

	friend big_integer operator+(const big_integer&, const big_integer&);
	friend big_integer operator*(const big_integer&, const big_integer&);
	friend big_integer operator/(const big_integer&, const big_integer&);
	friend big_integer operator%(const big_integer&, const big_integer&);
	friend big_integer operator-(const big_integer&, const big_integer&);

	friend class rational;
	friend std::ostream& operator<<(std::ostream&, const rational&);

	big_integer& operator++();
	big_integer operator++(int);

	big_integer& operator--();
	big_integer operator--(int);

	big_integer& operator+=(const big_integer&);
	big_integer& operator*=(const big_integer&);
	big_integer& operator/=(const big_integer& b);
	big_integer& operator%=(const big_integer&);
	big_integer& operator-=(const big_integer&);

	bool operator!() const;
	big_integer operator-();
	big_integer operator+();

	explicit operator bool() const;
	explicit operator int() const;
	explicit operator double() const;

	std::string to_string() const;
	big_integer Abs() const;
	big_integer& reverse();
	big_integer round(size_t) const;
	void swap(big_integer);

private:
	void CheckSignAndWrite(const char*);
	void HelperForCheckSingAndWrite(const char*, int);
	int CheckSignForChar(const char*);
	void HelperToString(std::string&) const;
	template<typename T>
	void CheckSignForOut(T&) const;

	void PredSummer(const big_integer&);
	void Summer(size_t, size_t, const big_integer&, bool Ifsmaller, size_t = 0);
	void HelperForSummer(int&, size_t, int = 0);

	void PredDestr(const big_integer&, size_t&);
	void Destructer(const big_integer&, size_t, size_t, size_t&, bool);
	void HelperDestructer1(const big_integer&, int&, size_t&, bool);
	void HelperDestructer2(const big_integer&, int&, size_t&, bool);

	void Multiply(const big_integer& a, const big_integer& b);
	bool HelperForMemcmp(const big_integer&, const big_integer&) const;

	bool HelperForOperatorLowandBig(const big_integer&, const big_integer&) const;

	void HelperForCtorFromDouble(double);

	template <typename T>
	void evaluate(T);

	bool getSign() const;
	const big_integer* getbiggerSize(const big_integer&) const;
	const big_integer* getSmallestSize(const big_integer&) const;
	big_integer join(const big_integer&) const;
	rational disjoin(size_t);
};
//                                         ОБЪЯВЛЕНИЯ СПЕЦИАЛИЗАЦИЙ
template<>
void big_integer::CheckSignForOut(std::string&) const;
template<>
void big_integer::CheckSignForOut(std::ostream&) const;


class rational
{
private:
	mutable big_integer nom;
	mutable big_integer denom;

public:
	rational();
	rational(const big_integer&, const big_integer&);
	explicit rational(const big_integer&);
	rational(int);
	rational(double);
	rational(const rational&);
	explicit rational(const std::string&);
	rational(const char*);
	rational& operator=(const rational&);

	friend std::ostream& operator<<(std::ostream&, const rational&);
	friend std::istream& operator>>(std::istream&, rational&);

	friend rational operator+(const rational&, const rational&);
	friend rational operator-(const rational&, const rational&);
	friend rational operator*(const rational&, const rational&);
	friend rational operator/(const rational&, const rational&);

	rational& operator+=(const rational&);
	rational& operator-=(const rational&);
	rational& operator*=(const rational&);
	rational& operator/=(const rational&);

	bool operator==(const rational&) const;
	std::partial_ordering operator<=>(const rational&) const;

	rational& operator++();
	rational operator++(int);

	rational& operator--();
	rational operator--(int);

	rational operator-();
	rational operator+();

	explicit operator bool() const;
	explicit operator double() const;

	std::string to_string() const;
	double asDecimal(size_t) const;

	rational round(size_t index = 0) const;
	rational Abs() const;

	void swap(const rational&);

private:
	void evaluate(size_t, const rational&);
	void evaluate2(size_t, const rational&, bool);
	void evaluate3(big_integer&, const rational*, bool);
	void evaluateForDivide(big_integer&, big_integer&, std::vector<int>&);
	const rational* solutionForNotEqualSize(rational& ,size_t, const rational&);
};

