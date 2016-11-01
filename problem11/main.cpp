#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>


const char data[] = 
"08 02 22 97 38 15 00 40 00 75 04 05 07 78 52 12 50 77 91 08 "
"49 49 99 40 17 81 18 57 60 87 17 40 98 43 69 48 04 56 62 00 "
"81 49 31 73 55 79 14 29 93 71 40 67 53 88 30 03 49 13 36 65 "
"52 70 95 23 04 60 11 42 69 24 68 56 01 32 56 71 37 02 36 91 "
"22 31 16 71 51 67 63 89 41 92 36 54 22 40 40 28 66 33 13 80 "
"24 47 32 60 99 03 45 02 44 75 33 53 78 36 84 20 35 17 12 50 "
"32 98 81 28 64 23 67 10 26 38 40 67 59 54 70 66 18 38 64 70 "
"67 26 20 68 02 62 12 20 95 63 94 39 63 08 40 91 66 49 94 21 "
"24 55 58 05 66 73 99 26 97 17 78 78 96 83 14 88 34 89 63 72 "
"21 36 23 09 75 00 76 44 20 45 35 14 00 61 33 97 34 31 33 95 "
"78 17 53 28 22 75 31 67 15 94 03 80 04 62 16 14 09 53 56 92 "
"16 39 05 42 96 35 31 47 55 58 88 24 00 17 54 24 36 29 85 57 "
"86 56 00 48 35 71 89 07 05 44 44 37 44 60 21 58 51 54 17 58 "
"19 80 81 68 05 94 47 69 28 73 92 13 86 52 17 77 04 89 55 40 "
"04 52 08 83 97 35 99 16 07 97 57 32 16 26 26 79 33 27 98 66 "
"88 36 68 87 57 62 20 72 03 46 33 67 46 55 12 32 63 93 53 69 "
"04 42 16 73 38 25 39 11 24 94 72 18 08 46 29 32 40 62 76 36 "
"20 69 36 41 72 30 23 88 34 62 99 69 82 67 59 85 74 04 36 16 "
"20 73 35 29 78 31 90 01 74 31 49 71 48 86 81 16 23 57 05 54 "
"01 70 54 71 83 51 54 69 16 92 33 48 61 43 52 01 89 19 67 48 "
;

template<typename It>
typename It::value_type max_product(It start, It end, int product_size)
{
	typedef typename It::value_type type;
	assert(product_size);

	auto max = type();
	type v = 1; //must be assignable to 1

	It block_start = start;

	int cnt = 1;
	for(It it = start; it != end; ++it, ++cnt) {
		if(*it == 0) { //no point to continue with 0
			cnt = 0;
			v = 1;
			block_start = it;
			++block_start;
			continue;
		}
		v *= *it;
		if(cnt % product_size == 0) {
			if(v > max) {
				max = v;
			}
			v /= *block_start; //moving window
			++block_start;
			--cnt;
		}
	}

	return max;
}

template<typename V = int>
struct matrix
{
	typedef V value_type;
	typedef std::vector<V> buffer;

	buffer d_;
	int cols_;
	int rows_;

	matrix(std::stringstream& data, int cols, int rows)
		:d_{std::istream_iterator<int>(data), std::istream_iterator<int>()}
		,cols_(cols)
		,rows_(rows)
	{
		assert(cols*rows == d_.size());
	}

	typename buffer::value_type const* operator[](int row) const
	{
		assert(cols_ * row < d_.size());
		return &d_[cols_ * row];
	}

	typedef typename buffer::const_iterator row_iterator;


	row_iterator row_begin(int row) const
	{
		return d_.begin() + row*cols_;
	}

	row_iterator row_end(int row) const
	{
		return d_.begin() + row*cols_ + cols_;
	}

	//used as a base for all iterator types
	struct matrix_iterator
	{
		typedef matrix::value_type value_type;

		const matrix *m_;
		int col_;
		int row_;
		matrix_iterator(const matrix& m, int col, int row)
			:m_(&m)
			,col_(col)
			,row_(row)
		{}

		value_type operator*() const
		{
			return (*m_)[row_][col_];
		}

		bool operator!=(const matrix_iterator& it) const
		{
			return row_ != it.row_ && col_ != it.col_;
		}
	};

	struct col_iterator : matrix_iterator
	{
		using matrix_iterator::row_;

		col_iterator(const matrix& m, int col, int row)
			:matrix_iterator(m, col, row)
		{}

		col_iterator& operator++()
		{
			++row_;
			return *this;
		}
	};

	col_iterator col_begin(int col) const
	{
		return col_iterator(*this, col, 0);
	}

	col_iterator col_end(int col) const
	{
		return col_iterator(*this, col, rows_);
	}

	struct diag_iterator : matrix_iterator
	{
		using matrix_iterator::row_;
		using matrix_iterator::col_;

		diag_iterator(const matrix& m, int col, int row)
			:matrix_iterator(m, col, row)
		{}

		diag_iterator& operator++()
		{
			++row_;
			++col_;
			return *this;
		}
	};


	diag_iterator diag_begin(int row, int col) const
	{
		return diag_iterator(*this, col, row);
	}
	diag_iterator diag_end() const
	{
		return diag_iterator(*this, cols_, rows_);
	}

	struct diag_cross_iterator : matrix_iterator
	{
		using matrix_iterator::row_;
		using matrix_iterator::col_;

		diag_cross_iterator(const matrix& m, int col, int row)
			:matrix_iterator(m, col, row)
		{}

		diag_cross_iterator& operator++()
		{
			--row_;
			++col_;
			return *this;
		}
	};

	diag_cross_iterator diag_cross_begin(int row, int col) const
	{
		return diag_cross_iterator(*this, col, row);
	}
	diag_cross_iterator diag_cross_end() const
	{
		return diag_cross_iterator(*this, cols_, -1);
	}
};

int main()
{
	std::cout << std::endl 
		<< "Project Euler - Problem 11" << std::endl
		<< "-------------------------" << std::endl;

	static const int product_size = 4;

	std::stringstream ss(data);


	matrix<int> m(ss, 20, 20);

	int max = 0;

	//left - right
	for(int i = 0; i != m.rows_; ++i) {
		int v = max_product(m.row_begin(i), m.row_end(i), product_size);
		if(v > max)
			max = v;
	}

	//up - down
	for(int i = 0; i != m.cols_; ++i) {
		int v = max_product(m.col_begin(i), m.col_end(i), product_size);
		if(v > max)
			max = v;
	}

	//row-side diags
	for(int i = 0; i != m.rows_; ++i) {
		int v = max_product(m.diag_begin(i, 0), m.diag_end(), product_size);
		if(v > max)
			max = v;
		if(i) {
			v = max_product(m.diag_cross_begin(i, 0), m.diag_cross_end(), product_size);
			if(v > max)
				max = v;
		}
	}

	//column-side diags
	for(int i = 1; i != m.cols_; ++i) {
		int v = max_product(m.diag_begin(0, i), m.diag_end(), product_size);
		if(v > max)
			max = v;
		v = max_product(m.diag_cross_begin(m.rows_-1, i), m.diag_cross_end(), product_size);
		if(v > max)
			max = v;
	}

	std::cout << "Answer: " << max << std::endl << std::endl;

	return 0;
}

