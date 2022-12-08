//============================================================================
// Name         : dnatypeb.hpp
// Author       : Roger Fraser
// Contributors :
// Version      : 1.00
// Copyright    : Copyright 2017 Geoscience Australia
//
//                Licensed under the Apache License, Version 2.0 (the "License");
//                you may not use this file except in compliance with the License.
//                You may obtain a copy of the License at
//               
//                http ://www.apache.org/licenses/LICENSE-2.0
//               
//                Unless required by applicable law or agreed to in writing, software
//                distributed under the License is distributed on an "AS IS" BASIS,
//                WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//                See the License for the specific language governing permissions and
//                limitations under the License.
//
// Description  : Type B uncertainty file io and helps
//============================================================================

#ifndef DNAIOTYPEB_H_
#define DNAIOTYPEB_H_

#if defined(_MSC_VER)
	#if defined(LIST_INCLUDES_ON_BUILD) 
		#pragma message("  " __FILE__) 
	#endif
#endif

#include <include/io/dnaiobase.hpp>
#include <include/config/dnatypes.hpp>
#include <include/functions/dnastrmanipfuncs.hpp>
#include <include/functions/dnatemplatematrixfuncs.hpp>
#include <include/functions/dnaiostreamfuncs.hpp>
#include <include/math/dnamatrix_contiguous.hpp>

using namespace std;
using namespace boost;

using namespace dynadjust::math;

namespace dynadjust {
namespace iostreams {


/////////////////////////////////////////////////////////////
// Custom type to manage cartesian plate motion model parameters
template <class T1 = UINT32, class T2 = double, class T3 = matrix_2d>
struct type_b_uncertainty_t
{
	T1 station_id;
	T3 type_b;

	type_b_uncertainty_t()
		: station_id(0) {
		type_b.redim(3, 3);
	}
	
	void set_station_id(const T1& stn=0) { station_id =stn; }	

	void set_typeb_values_3d(const T2& e, const T2& n, const T2& u) {
		type_b.put(0, 0, e);
		type_b.put(1, 1, n);
		type_b.put(2, 2, u);
	}

	bool operator== (const type_b_uncertainty_t<T1, T2, T3>& rhs) {
		return equal(station_id, rhs.station_id);
	}

	bool operator< (const type_b_uncertainty_t<T1, T2, T3>& rhs) const
	{
		return station_id < rhs.station_id;
	}
};

typedef type_b_uncertainty_t<UINT32, double, matrix_2d> type_b_uncertainty;
typedef vector<type_b_uncertainty> v_type_b_uncertainty;
typedef v_type_b_uncertainty::iterator it_type_b_uncertainty;
/////////////////////////////////////////////////////////////

class dna_io_tbu : public dna_io_base
{
public:
	dna_io_tbu(void) {};
	dna_io_tbu(const dna_io_tbu& tbu) : dna_io_base(tbu) {};
	virtual ~dna_io_tbu(void) {};

	dna_io_tbu& operator=(const dna_io_tbu& rhs);

	void read_tbu_header(std::ifstream* ptr, string& version, INPUT_DATA_TYPE& idt);

	void load_tbu_file(const string& tbu_filename, v_type_b_uncertainty& type_b_uncertainties, v_string_uint32_pair& vStnsMap);
	void load_tbu_argument(const string& argument, type_b_uncertainty& type_b_uncertainties);

	void identify_station_id(const string& stn_str, UINT32& stn_id, v_string_uint32_pair& vStnsMap);

	void validate_typeb_values(const string& argument, vstring& typeBUncertainties);
	void assign_typeb_values_global(const vstring& typeBUncertainties, type_b_uncertainty& type_b);
	void assign_typeb_values_local(const vstring& typeBUncertainties, type_b_uncertainty& type_b);

	void reduce_uncertainties_global(type_b_uncertainty& type_b, matrix_2d& var_cart, stn_t& bstBinaryRecord);
	void reduce_uncertainties_local(v_type_b_uncertainty& type_b, vstn_t& bstBinaryRecords);

protected:

};

}	// namespace iostreams
}	// namespace dynadjust

#endif /* DNAIOTYPEB_H_ */
